#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <fcntl.h>
#include <errno.h>

#include <sys/stat.h>
#include <sys/types.h>

#include "util.h"
#include "glad.h"
#include "math.h"
#include "mesh.h"
#include "camera.h"

#include <SDL.h>

struct input {
	SDL_Scancode keys[SDL_NUM_SCANCODES];
	int buttons[8];
	double xpos, ypos;
	double xinc, yinc;
} input, last_input;

static inline int
key_pressed(SDL_Scancode key)
{
	return input.keys[key] == SDL_PRESSED;
}

static inline int
on_key_pressed(SDL_Scancode key)
{
	return key_pressed(key) && input.keys[key] != last_input.keys[key];
}

static inline int
mouse_click(int b)
{
	return input.buttons[b] == SDL_PRESSED;
}

static inline int
on_mouse_click(int b)
{
	return mouse_click(b) && input.buttons[b] != last_input.buttons[b];
}

static void dbg_cross(vec3 at, vec3 scale, quaternion rot, vec3 color);
static void dbg_mark(vec3 at, vec3 scale, vec3 color);
static void dbg_line(vec3 a, vec3 b, vec3 color);

struct texture {
	GLuint id;
	GLenum type;
	GLenum unit;
	size_t width, height;
};

SDL_Window *window;
SDL_GLContext context;
double time_start;
float game_fps;
static int need_exit;

enum state {
	MENU,
	PLAY,
	FLAG,
} game_state = MENU;

char *argv0;
unsigned int width = 1080;
unsigned int height = 800;

static int dbg_en = 0;
static int dbg_on = 0;

static char flag_value[32];
struct camera cam;
float fog_density = 0.1;
vec3 fog_color = {0};
float player_speed = 3;
vec3 player_pos;
vec3 last_pos;
vec3 player_eye = {0, 1.5, 0};
vec3 player_dir;

const char *server_host;
const char *server_port;
const char *username;

int server_fd = -1;

struct shader {
	GLuint prog;
	const char *vert;
	const char *frag;
};

struct mesh cross_mesh;
struct mesh sphere_mesh;
struct mesh gui_mesh;
struct mesh wall_mesh;
#include "wall.msh"
struct mesh tile_mesh;
#include "tile.msh"
struct mesh hole_mesh;
#include "hole.msh"
struct mesh flag_mesh;
#include "flag.msh"

const char *const proj_vert =
	"#version 300 es\n"
	"precision highp float;\n"
	"precision highp int;\n"
	"in vec3 in_pos;\n"
	"in vec3 in_normal;\n"
	"in vec2 in_tex;\n"
	"out vec3 normal;\n"
	"out vec2 texcoord;\n"
	"out vec3 w_position;\n"
	"uniform mat4 proj;\n"
	"uniform mat4 model;\n"
	"uniform mat4 view;\n"
	"void main(){\n"
	"vec4 p = model * vec4(in_pos, 1.0);\n"
	"w_position = p.xyz;\n"
	"gl_Position = proj * view * p;\n"
	"texcoord = vec2(in_tex.x, 1.0 - in_tex.y);\n"
	"normal = (model * vec4(in_normal, 0.0)).xyz;\n"
	"}\n";

struct shader shaders[] = {
	{ .vert = proj_vert,
	  .frag =
	  "#version 300 es\n"
	  "precision highp float;\n"
	  "precision highp int;\n"
	  "in vec3 normal;\n"
	  "in vec2 texcoord;\n"
	  "in vec3 w_position;\n"
	  "out vec4 o_col;\n"
	  "uniform sampler2D tex;\n"
	  "uniform float fog_density;\n"
	  "uniform vec3 fog_color;\n"
	  "void main(){\n"
	  "vec2 uv = texcoord;\n"
	  "vec3 col = texture(tex, uv).rgb;\n"
	  "float z = gl_FragCoord.z / gl_FragCoord.w;\n"
	  "float fog = exp(-fog_density * z * z);\n"
	  "col = mix(fog_color, col, clamp(fog, 0.0, 1.0));\n"
	  "col = mix(col, fog_color, clamp(-0.5*(floor(w_position.y*16.0)/16.0), 0.0, 1.0));\n"
	  "o_col = vec4(col, 1.0);\n"
	  "}\n",
	},
	{ .vert = proj_vert,
	  .frag =
	  "#version 300 es\n"
	  "precision highp float;\n"
	  "precision highp int;\n"
	  "out vec4 o_col;\n"
	  "uniform vec3 color;\n"
	  "void main(){\n"
	  "o_col = vec4(color, 1.0);\n"
	  "}\n",
	},
	{ .vert =
	  "#version 300 es\n"
	  "precision highp float;\n"
	  "precision highp int;\n"
	  "uniform mat4 proj;\n"
	  "in vec3 in_pos;\n"
	  "in vec2 in_tex;\n"
	  "out vec2 uv;\n"
	  "void main(){\n"
	  "gl_Position = proj * vec4(in_pos.xy, -1.0, 1.0);\n"
	  "uv = vec2(in_tex.x, 1.0 - in_tex.y);\n"
	  "}\n",
	  .frag =
	  "#version 300 es\n"
	  "precision highp float;\n"
	  "precision highp int;\n"
	  "out vec4 o_col;\n"
	  "uniform vec3 color;\n"
	  "uniform sampler2D tex;\n"
	  "uniform vec4 texp;\n"
	  "uniform float weig;\n"
	  "in vec2 uv;\n"
	  "void main(){\n"
	  "float a = texture(tex, texp.xy + uv * texp.zw).r;\n"
	  "if ((1.0-weig) < max(a, 0.01))\n"
//	  "vec3 col = color * step(1.0-weig, max(a, 0.01));\n"
	  "o_col = vec4(color, 1.0);\n"
	  "else discard;\n"
	  "}\n",
	},
};

#define shader_count LEN(shaders)
struct shader *shader;

#include "qoi.h"
struct texture wall_tex;
#include "wall.qoi.c"
struct texture text_tex;
#include "ascii.qoi.c"

static void fini(void);
static void init(void);
static void try_connect(void);

static void
die(const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	va_end(ap);

	fini();
	exit(1);
}

#define MSEC_PER_SEC 1000
static double
get_time(void)
{
	return SDL_GetTicks() / (double) MSEC_PER_SEC;
}


#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include "proto.h"

static int client_connect(const char *host, const char *port)
{
	struct addrinfo hints = { 0 };
	struct addrinfo *addrs, *p;
	int ret;
	int sfd;

	hints.ai_family = AF_UNSPEC; /* or AF_INET */
	hints.ai_socktype = SOCK_STREAM; /* or DGRAM for UDP ? */
	hints.ai_protocol = 0;
	hints.ai_canonname = NULL;
	hints.ai_addr = NULL;
	hints.ai_next = NULL;

	ret = getaddrinfo(host, port, &hints, &addrs);
	if (ret != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(ret));
		return -1;
	}

	/* find the first address that successfully connect */
	for (sfd = -1, p = addrs; p != NULL; p = p->ai_next) {
		sfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
		if (sfd == -1)
			continue;
		ret = connect(sfd, p->ai_addr, p->ai_addrlen);
		if (ret != -1)
			break; /* success */
		close(sfd); /* try next entry */
	}
	freeaddrinfo(addrs);
	if (p == NULL) {
		/* tried all the addresses */
		fprintf(stderr, "could not connect\n");
		return -1;
	}

	return sfd;
}

static struct texture
create_2d_tex_f(size_t w, size_t h, GLenum iformat, GLenum tformat, GLenum type, void *data)
{
	struct texture tex;
	static GLuint unit = 0;

	tex.unit = unit++;
	tex.type = GL_TEXTURE_2D;
	tex.width = w;
	tex.height = h;

	glGenTextures(1, &tex.id);
	glBindTexture(tex.type, tex.id);

	glTexParameteri(tex.type, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(tex.type, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(tex.type, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(tex.type, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(tex.type, 0, iformat, w, h, 0, tformat, type, data);

	return tex;
}

static char logbuf[4096];
static GLsizei logsize;

static int
shader_compile(GLuint shd, const GLchar *txt, GLint len)
{
	int ret;

	glShaderSource(shd, 1, &txt, &len);
	glCompileShader(shd);
	glGetShaderiv(shd, GL_COMPILE_STATUS, &ret);
	if (!ret) {
		glGetShaderInfoLog(shd, sizeof(logbuf), &logsize, logbuf);
		printf("--- ERROR ---\n%s", logbuf);
	}
	return ret;
}

static int
shader_load(struct shader *s)
{
	GLuint nprg = glCreateProgram();
	GLuint vshd = glCreateShader(GL_VERTEX_SHADER);
	GLuint fshd = glCreateShader(GL_FRAGMENT_SHADER);
	int ret;

	ret = shader_compile(vshd, s->vert, strlen(s->vert));
	if (ret)
		ret = shader_compile(fshd, s->frag, strlen(s->frag));
	if (!ret)
		goto delete;

	glAttachShader(nprg, vshd);
	glAttachShader(nprg, fshd);
	glLinkProgram(nprg);
	glGetProgramiv(nprg, GL_LINK_STATUS, &ret);
	if (!ret) {
		glGetProgramInfoLog(nprg, sizeof(logbuf), &logsize, logbuf);
		printf("--- ERROR ---\n%s", logbuf);
		goto delete;
	}

	if (s->prog)
		glDeleteProgram(s->prog);
	s->prog = nprg;
	return 0;
delete:
	glDeleteProgram(nprg);
	glDeleteShader(vshd);
	glDeleteShader(fshd);
	return 1;
}

static void
shader_init(void)
{
	size_t i;
	for (i = 0; i < LEN(shaders); i++) {
		if (shader_load(&shaders[i]))
			die("fail to initialize shader %d\n", i);
	}
	printf("shader init done\n");
}

static void
mesh_init(void)
{
	mesh_load(&wall_mesh, wall_count, GL_TRIANGLES, wall_vert, wall_norm, wall_texc);
	mesh_load(&tile_mesh, tile_count, GL_TRIANGLES, tile_vert, tile_norm, tile_texc);
	mesh_load(&hole_mesh, hole_count, GL_TRIANGLES, hole_vert, hole_norm, hole_texc);
	mesh_load(&flag_mesh, flag_count, GL_TRIANGLES, flag_vert, flag_norm, flag_texc);
	mesh_load_quad(&gui_mesh, 1.0, 1.0);
	mesh_load_cross(&cross_mesh, 1.0);
	mesh_load_bounding_sphere(&sphere_mesh, 1.0);

	printf("mesh init done\n");
}

static void
texture_init(void)
{
	void *data;
	qoi_desc desc;

	data = qoi_decode(wall_qoi, wall_qoi_len, &desc, 3);
	wall_tex = create_2d_tex_f(desc.width, desc.height, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE, data);
	free(data);

	data = qoi_decode(ascii_qoi, ascii_qoi_len, &desc, 3);
	text_tex = create_2d_tex_f(desc.width, desc.height, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE, data);
	free(data);

	printf("texture init done\n");
}

static void
mesh_draw(struct mesh *m)
{
	if (m->index_count > 0)
		glDrawElements(m->primitive, m->index_count, GL_UNSIGNED_INT, 0);
	else
		glDrawArrays(m->primitive, 0, m->vertex_count);
}

static void
render_bind_camera(struct shader *s, struct camera *c)
{
	GLint proj, view;

	proj = glGetUniformLocation(s->prog, "proj");
	view = glGetUniformLocation(s->prog, "view");

	glUniformMatrix4fv(proj, 1, GL_FALSE, (float *)&c->proj.m);
	glUniformMatrix4fv(view, 1, GL_FALSE, (float *)&c->view.m);
}

struct map {
	uint32_t x, y;
	uint32_t type;
};
struct map blocks[0x1000];
size_t block_count = 0;

static int
map_cmp(const struct map *a, const struct map *b)
{
	if (a->x < b->x)
		return -1;
	if (a->x > b->x)
		return +1;
	if (a->y < b->y)
		return -1;
	if (a->y > b->y)
		return +1;
	return 0;
}
static int _map_cmp(const void *a, const void *b)
{
	return map_cmp(a, b);
}

static int
map_dist(const struct map *a)
{
	int x = a->x - player_pos.x;
	int y = a->y - player_pos.z;
	return (x < 0 ? -x : x) + (y < 0 ? -y : y);
}

static int _map_dist(const void *a, const void *b)
{
	return map_dist(a) - map_dist(b);
}

static void
map_sort(void)
{
	qsort(blocks, block_count, sizeof(blocks[0]), _map_cmp);
}

static void
map_free(size_t n)
{
	/* sort blocks by distance to the player */
	qsort(blocks, block_count, sizeof(blocks[0]), _map_dist);
	/* remove n farest blocks */
	if (block_count < n)
		block_count = 0;
	else
		block_count -= n;
	/* sort blocks back */
	qsort(blocks, block_count, sizeof(blocks[0]), _map_cmp);
}

static void
map_push(int x, int y, int val)
{
	struct map b = { .x = x, .y = y, .type = val };
	if (block_count < LEN(blocks))
	    blocks[block_count++] = b;
}

static void
map_init(void)
{
	block_count = 0;
}

static struct map *
map_at(int x, int y)
{
	struct map key = { .x = x, .y = y };
	if (x < 0 || y < 0)
		return NULL;
	return bsearch(&key, blocks, block_count, sizeof(blocks[0]), _map_cmp);
}

static int
type_at(int x, int y)
{
	struct map *b = map_at(x, y);
	return b ? (int)b->type : -1;
}

static void
dbg_mesh(struct mesh *m, vec3 at, vec3 scale, quaternion rot, vec3 color)
{
	if (!dbg_on) return;
	struct shader *s = &shaders[1];
	glUseProgram(s->prog);
	render_bind_camera(s, &cam);
	GLint position = glGetAttribLocation(s->prog, "in_pos");
	mesh_bind(m, position, -1, -1);
	GLint model = glGetUniformLocation(s->prog, "model");
	mat4 transform = mat4_transform_scale(at,
					 rot,
					 scale);
	glUniformMatrix4fv(model, 1, GL_FALSE, (float *)&transform.m);
	glUniform3f(glGetUniformLocation(s->prog, "color"), color.x, color.y, color.z);
	mesh_draw(m);
}
static void
dbg_cross(vec3 at, vec3 scale, quaternion rot, vec3 color)
{
	return dbg_mesh(&cross_mesh, at, scale, rot, color);
}
static void
dbg_mark(vec3 at, vec3 scale, vec3 color)
{
	dbg_cross(at, scale, QUATERNION_IDENTITY, color);
}
static void
dbg_line(vec3 a, vec3 b, vec3 color)
{
	vec3 dir = vec3_sub(b, a);
	vec3 pos = vec3_add(a, vec3_mult(0.5, dir));
	vec3 scale = {0, 0, vec3_norm(dir) * 0.5};
	quaternion rot = quaternion_look_at(dir, (vec3){0,1,0});
	dbg_cross(pos, scale, rot, color);
}
static void
dbg_circ(vec3 at, vec3 scale, vec3 color)
{
	return dbg_mesh(&sphere_mesh, at, scale, QUATERNION_IDENTITY, color);
}

static void
gui_draw(int x, int y, int w, int h, vec3 color)
{
	struct mesh *m = &gui_mesh;
	struct shader *s = &shaders[2];
	glUseProgram(s->prog);
	GLint pos = glGetAttribLocation(s->prog, "in_pos");
	GLint tex = glGetAttribLocation(s->prog, "in_tex");
	GLint proj = glGetUniformLocation(s->prog, "proj");
	GLint weig = glGetUniformLocation(s->prog, "weig");
	GLfloat ortho[4][4] = {
		{2.0f, 0.0f, 0.0f, 0.0f},
		{0.0f, 2.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 1.0f, 0.0f},
		{-1.0f, 1.0f, 0.0f, 1.0f},
	};
	y += h;
	ortho[0][0] = 2.0f * ((GLfloat)w / (GLfloat)width);
	ortho[1][1] = 2.0f * ((GLfloat)h / (GLfloat)height);
	ortho[3][0] = -1.0 + 2.0f * ((GLfloat)x / (GLfloat)width);
	ortho[3][1] = 1.0f -2.0f * ((GLfloat)y / (GLfloat)height);

	mesh_bind(m, pos, -1, tex);
	glUniform1f(weig, 1.0);
	glUniform3f(glGetUniformLocation(s->prog, "color"), color.x, color.y, color.z);
	glUniformMatrix4fv(proj, 1, GL_FALSE, &ortho[0][0]);
	mesh_draw(m);
}

static void
gui_text(int x, int y, const char *t, vec3 color)
{
	struct mesh *m = &gui_mesh;
	struct shader *s = &shaders[2];
	glUseProgram(s->prog);
	GLint pos = glGetAttribLocation(s->prog, "in_pos");
	GLint tex = glGetAttribLocation(s->prog, "in_tex");
	GLint proj = glGetUniformLocation(s->prog, "proj");
	GLint weig = glGetUniformLocation(s->prog, "weig");
	GLint texp = glGetUniformLocation(s->prog, "texp");
	GLint tex_loc = glGetUniformLocation(s->prog, "tex");
	GLfloat ortho[4][4] = {
		{2.0f, 0.0f, 0.0f, 0.0f},
		{0.0f, 2.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 1.0f, 0.0f},
		{-1.0f, 1.0f, 0.0f, 1.0f},
	};
	const int w = 24;
	mesh_bind(m, pos, -1, tex);
	glUniform1f(weig, 0.5);

	glUniform3f(glGetUniformLocation(s->prog, "color"), color.x, color.y, color.z);

	glActiveTexture(GL_TEXTURE0 + text_tex.unit);
	glUniform1i(tex_loc, text_tex.unit);
	glBindTexture(text_tex.type, text_tex.id);

	y += 32; // h
	while (*t) {
		ortho[0][0] = 2.0f * ((GLfloat)w / (GLfloat)width);
		ortho[1][1] = 2.0f * ((GLfloat)32 / (GLfloat)height);
		ortho[3][0] = -1.0f + 2.0f * ((GLfloat)x / (GLfloat)width);
		ortho[3][1] = 1.0f - 2.0f * ((GLfloat)y / (GLfloat)height);

		glUniformMatrix4fv(proj, 1, GL_FALSE, &ortho[0][0]);

		if (*t >= ' ' && *t <= '~') {
			int i = *t - ' ';
			float tx = i % 16;
			float ty = i / 16;
			glUniform4f(texp, tx / 16.0f, ty / 6.0f, 1.0/16.0, 1.0/6.0);
			mesh_draw(m);
		}

		t++;
		x += w;
	}
}


static void
render(void)
{
	GLint position;
	GLint texcoord;
	GLint id;
	struct shader *s = &shaders[0];
	struct mesh *m, *last_mesh = NULL;
	quaternion quad[4] = {
		QUATERNION_IDENTITY,
		quaternion_axis_angle(VEC3_AXIS_Y, 0.5*M_PI),
		quaternion_axis_angle(VEC3_AXIS_Y, 1.0*M_PI),
		quaternion_axis_angle(VEC3_AXIS_Y, 1.5*M_PI),
	};

	glUseProgram(s->prog);
	render_bind_camera(s, &cam);
	position = glGetAttribLocation(s->prog, "in_pos");
	texcoord = glGetAttribLocation(s->prog, "in_tex");

	id = glGetUniformLocation(s->prog, "fog_color");
	glUniform3f(id, fog_color.x, fog_color.y, fog_color.z);
	id = glGetUniformLocation(s->prog, "fog_density");
	glUniform1f(id, fog_density);

	GLint tex_loc = glGetUniformLocation(s->prog, "tex");
	glActiveTexture(GL_TEXTURE0 + wall_tex.unit);
	glUniform1i(tex_loc, wall_tex.unit);
	glBindTexture(wall_tex.type, wall_tex.id);

	GLint model = glGetUniformLocation(s->prog, "model");

	for (size_t i = 0; i < block_count; i++) {
		struct map b = blocks[i];
		quaternion rot = quad[(b.y + b.x) % LEN(quad)];
		mat4 transform;
		switch (b.type) {
		case TYPE_WALL:
			m = &wall_mesh;
			break;
		case TYPE_HOLE:
			m = &hole_mesh;
			break;
		case TYPE_TILE:
			m = &tile_mesh;
			break;
		case TYPE_FLAG:
			m = &flag_mesh;
			break;
		default:
			/* not valid */
			continue;
		}

		transform = mat4_transform_scale((vec3){b.x,0.0,b.y},
						 rot,
						 (vec3){1,1,1});
		if (model >= 0)
			glUniformMatrix4fv(model, 1, GL_FALSE, (float *)&transform.m);

		if (last_mesh != m)
			mesh_bind(last_mesh = m, position, -1, texcoord);
		mesh_draw(m);
		if (b.type == TYPE_FLAG) {
			m = &tile_mesh;
			if (last_mesh != m)
				mesh_bind(last_mesh = m, position, -1, texcoord);
			mesh_draw(m);
		}
	}
}

static void
poll_input(void)
{
	SDL_Event e;
	int key;

	last_input = input;
	input.xinc = 0;
	input.yinc = 0;
	while (SDL_PollEvent(&e)) {
		switch (e.type) {
		case SDL_QUIT:
			need_exit = 1;
			break;
		case SDL_KEYUP:
		case SDL_KEYDOWN:
			key = e.key.keysym.scancode;
			if ((unsigned int)key < LEN(input.keys))
				input.keys[key] = e.key.state;
			break;
		case SDL_MOUSEMOTION:
			input.xpos = e.motion.x;
			input.ypos = e.motion.y;
			input.xinc += e.motion.xrel;
			input.yinc += e.motion.yrel;
			break;
		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
			input.buttons[e.button.button] = e.button.state;
			break;
		case SDL_WINDOWEVENT:
			break;
		}
	}
}

struct edge {
	vec2 a, b;
};

static inline vec2
vec2_normalize(vec2 v)
{
	float n = sqrt(vec2_dot(v, v));
	vec2 r = { v.x / n, v.y / n };
	return r;
}

static vec2
point_on_edge(vec2 p, struct edge e)
{
	vec2 t = vec2_sub(e.b, e.a);
	vec2 pa = vec2_sub(p, e.a); /* a -> p */
	vec2 pb = vec2_sub(p, e.b); /* b -> p */

	if (vec2_dot(pa, t) <= 0)
		return e.a; /* p is before a on the ab edge */

	if (vec2_dot(pb, t) >= 0)
		return e.b; /* p is after b on the ab edge */

	t = vec2_normalize(t);
	return vec2_add(e.a, vec2_mult(vec2_dot(t, pa), t));
}

static void
player_walk(float dt)
{
#define dbg_v3(v) (vec3){v.x,0.01,v.y}
	const float walk_speed = key_pressed(SDL_SCANCODE_LSHIFT) ?
		2.0 * player_speed : player_speed;
	const int N = 5;
	const float R = 0.25;
	int type[N][N];
	vec3 player_dst = vec3_add(player_pos, vec3_mult(walk_speed * dt, player_dir));
	vec2 cur = (vec2){player_pos.x, player_pos.z};
	vec2 dst = (vec2){player_dst.x, player_dst.z};
	size_t i, j;
	int x, y;
	int ox, oy;
	vec3 ss = (vec3){0.01,0.01,0.01};
	dbg_mark(dbg_v3(cur), ss, (vec3){0,1,0});
	dbg_mark(dbg_v3(dst), ss, (vec3){0,0,1});
	dbg_circ(dbg_v3(cur), (vec3){R,0,R}, (vec3){0,1,0});
	ox = cur.x - (N/2);
	oy = cur.y - (N/2);
	for (x = 0; x < N; x++) {
		for (y = 0; y < N; y++) {
			vec3 col;
			vec2 p = {ox + x, oy + y};
			type[x][y] = type_at(p.x, p.y);
			col = type[x][y] == TYPE_TILE ? (vec3){0,1,0} : (vec3){1,0,0};
			dbg_mark(dbg_v3(p), ss, col);
		}
	}

	for (j = 0; j < 4; j++) {
	vec2 coll;
	float deep = -1;
	for (x = 0; x < N; x++) {
		for (y = 0; y < N; y++) {
			float ex = type[x][y] == TYPE_HOLE ? 0.3 : 0.5;
			vec2 p = {ox + x, oy + y};
			/* quad centered in p */
			vec2 a = {p.x - ex, p.y - ex};
			vec2 b = {p.x + ex, p.y - ex};
			vec2 c = {p.x + ex, p.y + ex};
			vec2 d = {p.x - ex, p.y + ex};
			struct edge e[4] = {
				{ a, b },
				{ b, c },
				{ c, d },
				{ d, a },
			};
			if (type[x][y] == TYPE_TILE || type[x][y] == TYPE_FLAG)
				continue;
			for (i = 0; i < LEN(e); i++) {
				if (j == 0)
					dbg_line(dbg_v3(e[i].a), dbg_v3(e[i].b), (vec3){1,0,1});
				vec2 d = vec2_sub(dst, point_on_edge(dst, e[i])); /* distance to the edge */
				float depth = vec2_dot(d, d);
				if (depth < R * R) { /* intersect with circle */
					depth = sqrt(depth);
					if (R - depth > deep) {
						coll = vec2_mult(1.0 / depth, d); // d
						deep = R - depth;
					}
				}
			}
		}
	}
	if (deep > 0) {
		dst = vec2_add(dst, vec2_mult(deep, coll)); /* fix the position */
		/* some debug marking */
		dbg_mark(dbg_v3(dst), ss, (vec3){0,1,1});
		dbg_circ(dbg_v3(dst), (vec3){R,0,R}, (vec3){0,1,1});
	}
	}

	/* apply the new position */
	player_pos.x = dst.x;
	player_pos.z = dst.y;
}

static int
mouse_in(int x, int y, int w, int h)
{
	return x <= input.xpos && input.xpos <= (x+w)
		&& y <= input.ypos && input.ypos <= (y+h);
}

static int
gui_button(int x, int y, const char *text)
{
	int w = 300;
	if (mouse_in(x, y, w, 32)) {
		gui_draw(x, y, w, 32, (vec3){0.4,0.4,0.0});
	} else {
		gui_draw(x, y, w, 32, (vec3){0.2,0.2,0.2});
	}
	gui_text(x, y, text, (vec3){1,1,1});

	return mouse_in(x, y, w, 32) && on_mouse_click(1);
}

static void
gui_menu(void)
{
	int x = (width - 200) / 2;
	int y = 64;

	if (gui_button(x, y + 64 * 0, "continue")) {
		game_state = PLAY;
	}
	if (gui_button(x, y + 64 * 1, server_fd >= 0 ? "connected" : "reconnect" )) {
		try_connect();
	}
	if (gui_button(x, y + 64 * 2, "quit")) {
		need_exit = 1;
	}
	if (dbg_en && gui_button(x, y + 64 * 3, dbg_on ? "debug on" : "debug off")) {
		dbg_on = !dbg_on;
	}

	if (on_key_pressed(SDL_SCANCODE_ESCAPE))
		game_state = PLAY;

	gui_draw(input.xpos, input.ypos, 4, 4, (vec3){1,1,0});
}

static void
gui_flag(void)
{
	int x = (width - 200) / 3;
	int y = 128;
	int w = 24;

	gui_draw(x - w, y, strlen(flag_value) * w + w, 32, (vec3){0.1,0.1,0.1});
	gui_text(x, y, flag_value, (vec3){1,1,1});
}

static void
gui_dbg(void)
{
	int x = 0;
	int y = height - 32;
	char str[32];
	int w = 24;
	snprintf(str, sizeof(str), "x:%.3f y:%.3f", player_pos.x, player_pos.z);

	gui_draw(x, y, strlen(str) * w, 32, (vec3){0.1,0.1,0.1});
	gui_text(x, y, str, (vec3){1,1,1});

	x = 0;
	y = 0;
	snprintf(str, sizeof(str), "%3.f fps", game_fps);
	gui_draw(x, y, strlen(str) * w, 32, (vec3){0.1,0.1,0.1});
	gui_text(x, y, str, (vec3){1,1,1});
}

static void
gui_net(void)
{
	int x = width;
	int y = 16;
	int w = 24;
	const char *m = "connection error";
	if (server_fd >= 0)
		return;
	x = width - strlen(m) * w;
	gui_draw(x, y, strlen(m) * w, 32, (vec3){0.1,0.1,0.1});
	gui_text(x, y, m, (vec3){1,0,0});
}

static void
player_move(float dt)
{
	const float mouse_speed = 0.002;
	int dir_forw = 0, dir_left = 0;
	vec3 dir;
	vec3 left = camera_get_left(&cam);
	vec3 forw = vec3_cross(left, VEC3_AXIS_Y);
	float dx, dy;

	if (key_pressed(SDL_SCANCODE_W))
		dir_forw += 1;
	if (key_pressed(SDL_SCANCODE_S))
		dir_forw -= 1;
	if (key_pressed(SDL_SCANCODE_A))
		dir_left += 1;
	if (key_pressed(SDL_SCANCODE_D))
		dir_left -= 1;
	if (dir_forw || dir_left) {
		forw = vec3_mult(dir_forw, forw);
		left = vec3_mult(dir_left, left);
		dir = vec3_add(forw, left);
		dir = vec3_normalize(dir);
		player_dir = dir;
	} else {
		player_dir = VEC3_ZERO;
	}

	dx = input.xinc;
	dy = input.yinc;

	if (dx || dy) {
		float f = vec3_dot(VEC3_AXIS_Y, camera_get_dir(&cam));
		float u = vec3_dot(VEC3_AXIS_Y, camera_get_up(&cam));
		float a_dy = sin(mouse_speed * dy);
		float a_max = 0.1;

		camera_rotate(&cam, VEC3_AXIS_Y, -mouse_speed * dx);
		left = camera_get_left(&cam);
		left = vec3_normalize(left);

		/* clamp view to a_max angle */
		if (f > 0 && (u + a_dy) < a_max) {
			a_dy = a_max - u;
		} else if (f < 0 && (u - a_dy) < a_max) {
			a_dy = -(a_max - u);
		}
		camera_rotate(&cam, left, a_dy);
	}

	/* player_walk update player_pos */
	player_walk(dt);
	if (server_fd >= 0 && (last_pos.x != player_pos.x || last_pos.z != player_pos.z)) {
		if (send_mov(server_fd, player_pos.x, player_pos.z) > 0) {
			last_pos = player_pos;
		}
	}

}

static void
player_getmap(void)
{
	vec2 cur = {player_pos.x, player_pos.z};
	const int N = 15;
	int x, y;
	int ox = cur.x - (N/2);
	int oy = cur.y - (N/2);
	ssize_t n;
	size_t count = 0;

	if (server_fd < 0)
		return;

	for (x = 0; x < N; x++) {
		for (y = 0; y < N; y++) {
			if ((ox + x) < 0 || (oy + y) < 0)
				continue;
			if (!map_at(ox + x, oy + y)) {
				n = send_get(server_fd, ox + x, oy + y);
				count++;
				if (n < 0) {
					fprintf(stderr, "server connection closed: %s\n", strerror(errno));
					close(server_fd);
					server_fd = -1;
					return;
				}
			}
		}
	}
	if (block_count + count > LEN(blocks)) {
		/* no space left for all the requested new blocks */
		/* free some space */
		fprintf(stderr, "freeing %ld space\n", MAX(1024, count));
		map_free(MAX(1024, count));
	}
}

static void
flycam_move(float dt)
{
	vec3 forw = camera_get_dir(&cam);
	vec3 left = camera_get_left(&cam);
	vec3 dir = { 0 };
	int fly_forw, fly_left;
	float dx, dy;
	float speed;

	if (key_pressed(SDL_SCANCODE_LSHIFT))
		speed = 10 * dt;
	else
		speed = 2 * dt;

	fly_forw = 0;
	if (key_pressed(SDL_SCANCODE_W))
		fly_forw += 1;
	if (key_pressed(SDL_SCANCODE_S))
		fly_forw -= 1;

	fly_left = 0;
	if (key_pressed(SDL_SCANCODE_A))
		fly_left += 1;
	if (key_pressed(SDL_SCANCODE_D))
		fly_left -= 1;

	if (fly_forw || fly_left) {
		forw = vec3_mult(fly_forw, forw);
		left = vec3_mult(fly_left, left);
		dir = vec3_add(forw, left);
		dir = vec3_normalize(dir);
		dir = vec3_mult(speed, dir);
		camera_move(&cam, dir);
	}

	dx = input.xinc;
	dy = input.yinc;

	if (dx || dy) {
		camera_rotate(&cam, VEC3_AXIS_Y, -0.001 * dx);
		left = camera_get_left(&cam);
		left = vec3_normalize(left);
		camera_rotate(&cam, left, 0.001 * dy);
	}
}

static void
try_connect(void)
{
	int new_fd;
	printf("trying to connect to %s:%s\n", server_host, server_port);
	new_fd = client_connect(server_host, server_port);
	if (new_fd < 0) {
		fprintf(stderr, "failed to connect to %s %s\n", server_host, server_port);
		return;
	}
	if (server_fd >= 0)
		close(server_fd);
	server_fd = new_fd;

	map_init();
	send_hey(server_fd, username);
}

static void
poll_server(void)
{
	int map_mod = 0;
	struct cmd_msg m;

	if (server_fd < 0)
		return;
	while (recv_rdy(server_fd, &m) == sizeof(m)) {
		recv_cmd(server_fd, &m);
//		printf("recv cmd %d '%s'\n", m.cmd, cmd_name(m.cmd));
		switch (m.cmd) {
		case POS:
			player_pos.x = m.pos.x;
			player_pos.z = m.pos.y;
			break;
		case GET:
			map_push(m.get.x, m.get.y, m.get.val);
			map_mod = 1;
			break;
		case RSP:
			// trigger only rsp with flag in it
			if (m.rsp.msg[0] == 2) {
				snprintf(flag_value, sizeof(flag_value), "%s", &m.rsp.msg[1]);
				printf("received a message: %s\n", flag_value);
				game_state = FLAG;
			}
			break;
		}
	}
	if (errno == ECONNRESET) {
		fprintf(stderr, "server connection closed: %s\n", strerror(errno));
		close(server_fd);
		server_fd = -1;
	}

	if (map_mod)
		map_sort();
}

static void
update(float dt)
{
	static enum state last_state = -1;
	if (key_pressed(SDL_SCANCODE_LCTRL) && on_key_pressed(SDL_SCANCODE_R)) {
		try_connect();
	}
	if (key_pressed(SDL_SCANCODE_LCTRL) && on_key_pressed(SDL_SCANCODE_P)) {
		map_free(block_count);
	}
	if (last_state != game_state) {
		switch (game_state) {
		case MENU:
			SDL_SetRelativeMouseMode(SDL_FALSE);
			SDL_ShowCursor(1);
			break;
		case PLAY:
			SDL_SetRelativeMouseMode(SDL_TRUE);
			SDL_ShowCursor(0);
			break;
		case FLAG:
			SDL_SetRelativeMouseMode(SDL_FALSE);
			SDL_ShowCursor(1);
		}
	}
	switch (game_state) {
	case MENU:
		render();
		gui_menu();
		break;
	case PLAY:
		player_getmap();
		player_move(dt);
		camera_set_position(&cam, vec3_add(player_pos, player_eye));
//	flycam_move(dt);
		render();
		if (on_key_pressed(SDL_SCANCODE_ESCAPE))
			game_state = MENU;

		break;
	case FLAG:
		render();
		gui_flag();
		if (on_key_pressed(SDL_SCANCODE_ESCAPE))
			game_state = PLAY;
		break;
	}
	if (dbg_on)
		gui_dbg();
	gui_net();
}

static void
init(void)
{
	if (SDL_InitSubSystem(SDL_INIT_VIDEO))
		die("SDL init failed: %s\n", SDL_GetError());

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, SDL_TRUE);
	SDL_GL_SetSwapInterval(1);

	window = SDL_CreateWindow(argv0, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
				  width, height,
				  SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
				  | SDL_WINDOW_INPUT_FOCUS
				  | SDL_WINDOW_MOUSE_FOCUS
				  | SDL_WINDOW_MAXIMIZED
				  );

	if (!window)
		die("Failed to create window: %s\n", SDL_GetError());

	context = SDL_GL_CreateContext(window);
	if (!context)
		die("Failed to create openGL context: %s\n", SDL_GetError());

	if (!gladLoadGLLoader((GLADloadproc) SDL_GL_GetProcAddress))
		die("GL init failed\n");

	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	SDL_GL_SwapWindow(window);

	time_start = get_time();

	shader_init();
	mesh_init();
	texture_init();
	camera_init(&cam, 1.05, 1);
	camera_set_znear(&cam, 0.1);
	camera_set(&cam, (vec3){-5, 1.0, 0}, QUATERNION_IDENTITY);
	camera_look_at(&cam, (vec3){0.0, 1.0, 0}, VEC3_AXIS_Y);

	try_connect();
}

static void
fini(void)
{
	if (server_fd >= 0)
		close(server_fd);
}

static void
usage(void)
{
	printf("usage: %s hostname port [username]\n", argv0);
	exit(1);
}

static const char *
name(void)
{
	const char *s;
	s = getenv("USER");
	if (s) return s;
	s = getenv("LOGIN");
	if (s) return s;

	return "l4m3r";
}
int
main(int argc, char **argv)
{
	double prev_time, curr_time;
	argv0 = argv[0];

	server_host = "localhost";
	server_port = "12345";
	username = name();

	if (argc < 3)
		usage();
	server_host = argv[1];
	server_port = argv[2];
	if (argc > 3)
                username = argv[3];

	init();
	SDL_SetRelativeMouseMode(SDL_TRUE);
	SDL_ShowCursor(0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);

	dbg_en = getenv("DEBUG") != NULL;

	game_fps = 60;
	curr_time = get_time();
	while (!need_exit) {
		int w, h;
		prev_time = curr_time;
		curr_time = get_time();
		SDL_GL_GetDrawableSize(window, &w, &h);
		width  = (w < 1) ? 1 : w;
		height = (h < 1) ? 1 : h;
		poll_input();
		poll_server();

		glViewport(0, 0, width, height);
		camera_set_ratio(&cam, width / (float) height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		update(curr_time - prev_time);
		SDL_GL_SwapWindow(window);
		if (curr_time != prev_time)
			game_fps = 0.5 * game_fps + 0.5 * (1.0 / (curr_time - prev_time));
	}
	fini();

	return 0;
}

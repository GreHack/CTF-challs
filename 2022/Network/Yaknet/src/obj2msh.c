#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#define MIN(a,b) ((a) < (b) ? (a) : (b))

typedef struct vec3 {
	float x, y, z;
} vec3;

struct file {
	ssize_t size;
	char *data;
};

static struct file
read_file(FILE *f)
{
	struct file file;
	size_t r, c, s = 0;
	char *d = NULL;

	do {
		d = realloc(d, c = s + 4096);
		r = fread(&d[s], 1, c - s, f);
		s += r;
	} while (!feof(f) && r > 0);

	file.size = s;
	file.data = d;
	return file;
}

struct obj_info {
	unsigned int vert_count;
	unsigned int texc_count;
	unsigned int norm_count;
	unsigned int face_count;
	const char *name;
};

struct vertex_index {
	unsigned int p;
	unsigned int t;
	unsigned int n;
};

/* We consider only triangulated faces */
struct face {
	struct vertex_index v0;
	struct vertex_index v1;
	struct vertex_index v2;
};

static struct obj_info
read_obj_info(struct file *file)
{
	struct obj_info info = {};
	size_t size = file->size;
	char *obj = file->data;
	char *line;
	size_t i, len = 0;
	size_t vcount = 0;
	size_t tcount = 0;
	size_t ncount = 0;
	size_t fcount = 0;

	/* count the number of vertices, normals, texcoord and faces */
	for (i = 0; i < size; i += len) {
		line = &obj[i];
		for (len = 0; i + len < size; len++)
			if (line[len] == '\n' || line[len] == '\0')
				break;
		if ((i + len) < size)
			line[len++] = '\0'; /* replace '\n' with '\0' */

		if (strncmp("v ", line, strlen("v ")) == 0) {
			vcount++;
		} else if (strncmp("vt ", line, strlen("vt ")) == 0) {
			tcount++;
		} else if (strncmp("vn ", line, strlen("vn ")) == 0) {
			ncount++;
		} else if (strncmp("f ", line, strlen("f ")) == 0) {
			fcount++;
		} else if (strncmp("o ", line, strlen("o ")) == 0) {
			info.name = line + strlen("o ");
		} else {
			/* ingore the line */
		}
	}
	info.vert_count = vcount;
	info.texc_count = tcount;
	info.norm_count = ncount;
	info.face_count = fcount;

	return info;
}

static void
load_obj(struct file *file, struct obj_info info,
	 size_t count, float *out_vert, float *out_norm, float *out_texc)
{
	size_t size = file->size;
	char *obj = file->data;
	char *line;
	int verbose = 0;
	size_t i, len = 0;
	size_t vcount = 0;
	size_t tcount = 0;
	size_t ncount = 0;
	size_t fcount = 0;
	vec3 *obj_vert;
	vec3 *obj_texc;
	vec3 *obj_norm;
	struct face *obj_face;
	float x, y, z;
	int p0, p1, p2, t0, t1, t2, n0, n1, n2;
	int n;

	/* allocate temporary buffer for parsing the obj file */
	obj_vert = malloc(info.vert_count * sizeof(*obj_vert));
	obj_texc = malloc(info.texc_count * sizeof(*obj_texc));
	obj_norm = malloc(info.norm_count * sizeof(*obj_norm));
	obj_face = malloc(info.face_count * sizeof(*obj_face));

	for (i = 0; i < size; i += len) {
		line = &obj[i];
		for (len = 0; (i + len) < size; len++)
			if (line[len] == '\n' || line[len] == '\0')
				break;
		if ((i + len) < size)
			line[len++] = '\0'; /* replace '\n' with '\0' */

		// TODO: line ending with / are continuated;
		if (strncmp("v ", line, strlen("v ")) == 0) {
			n = sscanf(line, "v %f %f %f", &x, &y, &z);
			if (n == 3) {
				if (verbose)
					printf("v %f %f %f\n", x, y, z);
				obj_vert[vcount++] = (vec3) {x, y, z};
			} else {
				fprintf(stderr, "Malformed line starting with 'v '\n");
			}
		} else if (strncmp("vt ", line, strlen("vt ")) == 0) {
			/*
			  We assume all textures are 2D textures:
			  We look for lines of the form
			  "vt u v \n"
			  with:
			  - u the value for the horizontal direction
			  - v the value for the vertical direction
			  TODO: accept 1D, 2D, 3D textures
			  i.e lines of form:
			  "vt u \n"
			  "vt u v \n"
			  "vt u v w \n"
			 */
			n = sscanf(line, "vt %f %f", &x, &y);
			if (n == 2) {
				if (verbose)
					printf("vt %f %f \n", x, y);
				obj_texc[tcount++] = (vec3) {x, y, 0};
			} else {
				fprintf(stderr, "Malformed line starting with 'vt '\n");
			}
		} else if (strncmp("vn ", line, strlen("vn ")) == 0) {
			n = sscanf(line, "vn %f %f %f", &x, &y, &z);
			if (n == 3) {
				if (verbose)
					printf("vn %f %f %f \n", x, y, z);
				obj_norm[ncount++] = (vec3) {x, y, z};
			} else {
				fprintf(stderr, "Malformed line starting with 'vn '\n");
			}
		} else if (strncmp("f ", line, strlen("f ")) == 0) {
			/* A face line starts with 'f' and is followed by triplets
			   of the form "vertex/texcoord/normal".
			   Example, for a mesh with triangular faces:
			   "f a1/b1/c1 a2/b2/c2 a3/b3/c3 \n"
			   With:
			   - a is the index of a vertex in the obj file vertex list
			   - b is the index of a texture coordinate in the obj texcoords list
			   - c is the index of a normal in the obj file normal list
			   Rem:
			   - in obj files, indices starts from 1
			   arrays indices start from 0 hence the Vertices[a-1] bellow.
			   - For now we only accept mesh with triangular faces.
			   Todo: accept more cellular division by adapting
			   format string fmt bellow.
			   - We assume texture coordinates and normal are present
			   Todo: accept faces lines of the form
			   - "f a1 a2 a3 \n"
			   - "f a1/b1 a2/b2 a3/b3 \n"
			   - "f a1/c1 a2/c2 a3/c3 \n"
			*/
			n = sscanf(line, "f %d/%d/%d %d/%d/%d %d/%d/%d",
				   &p0,&t0,&n0, &p1,&t1,&n1, &p2,&t2,&n2);
			if (n != 9) {
				/* without textcoord */
				t0 = t1 = t2 = 0;
				n = sscanf(line, "f %d//%d %d//%d %d//%d",
					   &p0,&n0, &p1,&n1, &p2,&n2);
			}
			if (n == 9 || n == 6) {
				struct face f = {{p0, t0, n0}, {p1, t1, n1},  {p2, t2, n2}};
				if (verbose)
					printf("f %d/%d/%d %d/%d/%d %d/%d/%d \n",
					       p0,t0,n0, p1,t1,n1, p2,t2,n2);
				obj_face[fcount++] = f;
			} else {
				fprintf(stderr, "Malformed line starting with 'f '\n");
			}
		} else {
			/* ingore the line */
		}
	}

	fcount = MIN(fcount, count);
	if (out_vert) {
		for (i = 0; i < fcount; i++) {
			out_vert[i*9 + 0] = obj_vert[obj_face[i].v0.p - 1].x;
			out_vert[i*9 + 1] = obj_vert[obj_face[i].v0.p - 1].y;
			out_vert[i*9 + 2] = obj_vert[obj_face[i].v0.p - 1].z;
			out_vert[i*9 + 3] = obj_vert[obj_face[i].v1.p - 1].x;
			out_vert[i*9 + 4] = obj_vert[obj_face[i].v1.p - 1].y;
			out_vert[i*9 + 5] = obj_vert[obj_face[i].v1.p - 1].z;
			out_vert[i*9 + 6] = obj_vert[obj_face[i].v2.p - 1].x;
			out_vert[i*9 + 7] = obj_vert[obj_face[i].v2.p - 1].y;
			out_vert[i*9 + 8] = obj_vert[obj_face[i].v2.p - 1].z;
		}
	}
	if (out_norm && ncount > 0) {
		for (i = 0; i < fcount; i++) {
			out_norm[i*9 + 0] = obj_norm[obj_face[i].v0.n - 1].x;
			out_norm[i*9 + 1] = obj_norm[obj_face[i].v0.n - 1].y;
			out_norm[i*9 + 2] = obj_norm[obj_face[i].v0.n - 1].z;
			out_norm[i*9 + 3] = obj_norm[obj_face[i].v1.n - 1].x;
			out_norm[i*9 + 4] = obj_norm[obj_face[i].v1.n - 1].y;
			out_norm[i*9 + 5] = obj_norm[obj_face[i].v1.n - 1].z;
			out_norm[i*9 + 6] = obj_norm[obj_face[i].v2.n - 1].x;
			out_norm[i*9 + 7] = obj_norm[obj_face[i].v2.n - 1].y;
			out_norm[i*9 + 8] = obj_norm[obj_face[i].v2.n - 1].z;
		}
	}
	if (out_texc && tcount > 0) {
		for (i = 0; i < fcount; i++) {
			out_texc[i*6 + 0] = obj_texc[obj_face[i].v0.t - 1].x;
			out_texc[i*6 + 1] = obj_texc[obj_face[i].v0.t - 1].y;
			out_texc[i*6 + 2] = obj_texc[obj_face[i].v1.t - 1].x;
			out_texc[i*6 + 3] = obj_texc[obj_face[i].v1.t - 1].y;
			out_texc[i*6 + 4] = obj_texc[obj_face[i].v2.t - 1].x;
			out_texc[i*6 + 5] = obj_texc[obj_face[i].v2.t - 1].y;
		}
	}

	free(obj_vert);
	free(obj_texc);
	free(obj_norm);
	free(obj_face);
}

static void
convert(FILE *s)
{
	struct file file = read_file(s);
	struct obj_info info = read_obj_info(&file);
	float *positions;
	float *normals;
	float *texcoords;
	size_t i, fcount;
	const char *name = info.name;

	fcount = info.face_count;
	/* keep positions outside of scrap/tmp zone */
	positions = malloc(fcount * 3 * 3 * sizeof(float));
	normals   = malloc(fcount * 3 * 3 * sizeof(float));
	texcoords = NULL;
	if (info.texc_count > 0)
		texcoords = malloc(fcount * 3 * 2 * sizeof(float));

	load_obj(&file, info, fcount, positions, normals, texcoords);

	printf("static size_t %s_count = %ld;\n", name, fcount * 3);
	printf("static float %s_vert[] = {\n", name);
	for (i = 0; i < fcount * 3 * 3; i += 3)
		printf("%f, %f, %f,\n", positions[i+0],positions[i+1],positions[i+2]);
	printf("};\n");
	printf("static float %s_norm[] = {\n", name);
	for (i = 0; i < fcount * 3 * 3; i += 3)
		printf("%f, %f, %f,\n", normals[i+0],normals[i+1],normals[i+2]);
	printf("};\n");
	printf("static float %s_texc[] = {\n", name);
	for (i = 0; i < fcount * 3 * 2; i += 2)
		printf("%f, %f,\n", texcoords[i+0],texcoords[i+1]);
	printf("};\n");
}

int main(int argc, char **argv)
{
	FILE *s = stdin;

	if (argc > 1) {
		s = fopen(argv[1], "r");
		if (!s) {
			perror(argv[1]);
			return 1;
		}
	}
	convert(s);

	return 0;
}

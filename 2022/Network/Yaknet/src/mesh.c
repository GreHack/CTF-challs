#include "mesh.h"

static struct bounding_volume
bounding_volume(size_t count, float *positions)
{
	struct bounding_volume bvol = { 0 };
	float len, max = 0;
	unsigned int i;
	vec3 pos;

	if (!positions)
		return bvol;

	for (i = 0; i < count; i++) {
		bvol.min.x = MIN(bvol.min.x, positions[i * 3 + 0]);
		bvol.min.y = MIN(bvol.min.y, positions[i * 3 + 1]);
		bvol.min.z = MIN(bvol.min.z, positions[i * 3 + 2]);
		bvol.max.x = MAX(bvol.max.x, positions[i * 3 + 0]);
		bvol.max.y = MAX(bvol.max.y, positions[i * 3 + 1]);
		bvol.max.z = MAX(bvol.max.z, positions[i * 3 + 2]);
	}

	/* full extent from one corner to the other */
	bvol.ext = vec3_mult(0.5, vec3_sub(bvol.max, bvol.min));
	/* half extent plus min is the offset from origin to center */
	bvol.off = vec3_add(bvol.ext, bvol.min);

	for (i = 0; i < count; i++) {
		pos.x = positions[i * 3 + 0];
		pos.y = positions[i * 3 + 1];
		pos.z = positions[i * 3 + 2];

		/* distance to the bounding volume's center */
		pos = vec3_sub(pos, bvol.off);
		len = vec3_dot(pos, pos);
		if (len > max)
			max = len;
	}
	bvol.radius = sqrt(max);

	return bvol;
}

static void
mesh_init_vbo(struct mesh *m, size_t count, GLenum primitive, int positions, int normals, int texcoords)
{
	GLuint vbo_count = 0;

	m->idx_positions = (positions) ? vbo_count++ : 0;
	m->idx_normals   = (normals)   ? vbo_count++ : 0;
	m->idx_texcoords = (texcoords) ? vbo_count++ : 0;

	glGenBuffers(vbo_count, m->vbo);

	m->vbo_count = vbo_count;
	m->vertex_count = count;
	m->index_count = 0;

	m->primitive = primitive;
}

static void
mesh_buffer(struct mesh *m, int idx, size_t nmemb, size_t size, void *data, GLenum usage)
{
	glBindBuffer(GL_ARRAY_BUFFER, m->vbo[idx]);
	glBufferData(GL_ARRAY_BUFFER, m->vertex_count * nmemb * size, data, usage);
}

void
mesh_load(struct mesh *m, size_t count, GLenum primitive, float *positions, float *normals, float *texcoords)
{
	mesh_init_vbo(m, count, primitive, positions != NULL, normals != NULL, texcoords != NULL);

	if (positions)
		mesh_buffer(m, m->idx_positions, 3, sizeof(float), positions, GL_STATIC_DRAW);

	if (normals)
		mesh_buffer(m, m->idx_normals, 3, sizeof(float), normals, GL_STATIC_DRAW);

	if (texcoords)
		mesh_buffer(m, m->idx_texcoords, 2, sizeof(float), texcoords, GL_STATIC_DRAW);

	glBindVertexArray(0);

	m->bounding = bounding_volume(count, positions);
}

void
mesh_index(struct mesh *m, size_t index_count, unsigned int *indices)
{

	if (!indices)
		return;

	m->idx_indices = m->vbo_count++;

	glGenBuffers(1, &m->vbo[m->idx_indices]);

	m->index_count = index_count;

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m->vbo[m->idx_indices]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m->index_count * sizeof(unsigned int), indices, GL_STATIC_DRAW);
}

void
mesh_bind(struct mesh *m, GLint position, GLint normal, GLint texture)
{
	if (position >= 0) {
		glBindBuffer(GL_ARRAY_BUFFER, m->vbo[m->idx_positions]);
		glVertexAttribPointer(position, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(position);
	}

	if (normal >= 0) {
		glBindBuffer(GL_ARRAY_BUFFER, m->vbo[m->idx_normals]);
		glVertexAttribPointer(normal, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(normal);
	}

	if (texture >= 0) {
		glBindBuffer(GL_ARRAY_BUFFER, m->vbo[m->idx_texcoords]);
		glVertexAttribPointer(texture, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(texture);
	}
}

void
mesh_free(struct mesh* m)
{
	if (m->vbo_count)
		glDeleteBuffers(m->vbo_count, m->vbo);
	m->vbo_count = 0;

	if (m->vao && glIsVertexArray(m->vao) == GL_TRUE)
		glDeleteVertexArrays(1, &m->vao);
}

void
mesh_load_cross(struct mesh *m, float x)
{
	float positions[] = {
		-x,  0,  0,
		 x,  0,  0,
		 0, -x,  0,
		 0,  x,  0,
		 0,  0, -x,
		 0,  0,  x,
	};

	mesh_load(m, 6, GL_LINES, positions, NULL, NULL);
}

void
mesh_load_quad(struct mesh *m, float x, float y)
{
	float positions[] = {
		 0,  0,  0,
		 x,  0,  0,
		 0,  y,  0,
		 x,  y,  0,
	};
	float texcoords[] = {
		0.0, 0.0,
		1.0, 0.0,
		0.0, 1.0,
		1.0, 1.0,
	};

	mesh_load(m, 4, GL_TRIANGLE_STRIP, positions, NULL, texcoords);
}

void
mesh_load_bounding_sphere(struct mesh *m, float radius)
{
	int step = 24;
	float positions[3 * 3 * (step + 1)];
	float a;
	int off, i;

	/* Simple representation of a sphere as 3 circles.
	 *
	 * Generate 3 circles, each perpendicular to each other.
	 * In order to use GL_LINE_LOOP every line segment must
	 * be continuous, eg keep the pen on the paper.
	 *
	 * The trick is:
	 * let says each circle can be broken in 4 part or quarter.
	 * we have 3 circles a,b and c to draw, the total parts are:
	 *   a1 a2 a3 a4   b1 b2 b3 b4   c1 c2 c3 c4
	 *
	 * Now, one solution to draw the 3 circles in one stroke is
	 * the following:
	 *  - start with the first circle:       a1 a2 a3 a4
	 *  - do 1 quarter of the second circle: b1
	 *  - do the last circle:                c1 c2 c3 c4
	 *  - finish the second circle:             b2 b3 b4
	 *
	 * In memory we have the following sequence:
	 *   a1 a2 a3 a4   b1 c1 c2 c3   c4 b2 b3 b4
	 */

	for (i = 0; i < step; i++) {
		a = 2 * M_PI * (i / (float) step);

		off = 3 * (i + 0 * step);
		positions[off + 0] = radius * sin(a);
		positions[off + 1] = radius * cos(a);
		positions[off + 2] = 0;

		if (i < (step / 4))
			off = 3 * (i + 1 * step);
		else
			off = 3 * (i + 2 * step);
		positions[off + 0] = 0;
		positions[off + 1] = radius * cos(a);
		positions[off + 2] = radius * sin(a);

		if (i < (step / 4))
			off = 3 * (i + 2 * step);
		else
			off = 3 * (i + 1 * step);
		positions[off + 0] = radius * cos(a);
		positions[off + 1] = 0;
		positions[off + 2] = radius * sin(a);
	}

	mesh_load(m, 3 * step, GL_LINE_LOOP, positions, NULL, NULL);
}

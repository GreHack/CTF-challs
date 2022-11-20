#include <stdlib.h>
#include "util.h"
#include "glad.h"
#include "math.h"

#define MESH_MAX_VBO 4

struct mesh {
	GLuint vao;
	GLuint vbo[MESH_MAX_VBO];
	int vbo_count;
	int idx_positions;
	int idx_normals;
	int idx_texcoords;
	int idx_indices;
	size_t vertex_count;
	size_t index_count;
	GLenum primitive;
	struct bounding_volume {
		vec3 min;
		vec3 max;
		vec3 ext;
		vec3 off; /* offset to the mesh origin */
		float radius; /* bounding sphere radius */
	} bounding;
	float *positions;
};

void mesh_load(struct mesh *m, size_t count, GLenum primitive, float *positions, float *normals, float *texcoords);
void mesh_index(struct mesh *m, size_t count, unsigned int *index);
void mesh_bind(struct mesh *m, GLint position, GLint normal, GLint texture);
void mesh_free(struct mesh *m);
void mesh_load_cross(struct mesh *m, float x);
void mesh_load_bounding_sphere(struct mesh *m, float r);
void mesh_load_quad(struct mesh *m, float x, float y);

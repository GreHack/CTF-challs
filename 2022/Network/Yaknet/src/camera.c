#include <math.h>
#include "util.h"
#include "camera.h"

static void camera_update_view(struct camera *c);
static void camera_update_proj(struct camera *c);

void
camera_init(struct camera* c, float fov, float ratio)
{
	c->position = VEC3_ZERO;
	c->rotation = QUATERNION_IDENTITY;
	c->view = MAT4_IDENTITY;
	c->proj = MAT4_IDENTITY;

	c->fov = fov;
	c->ratio = ratio;
	c->zNear = 0.4;
	c->zFar = 2000;

	c->proj = (mat4){ 0 };
	camera_update_view(c);
	camera_update_proj(c);
}

void
camera_set(struct camera *c, vec3 position, quaternion rotation)
{
	c->position = position;
	c->rotation = rotation;
	camera_update_view(c);
}

void
camera_set_znear(struct camera *c, float z)
{
	c->zNear = z;
	camera_update_proj(c);
}

void
camera_set_position(struct camera *c, vec3 position)
{
	camera_set(c, position, c->rotation);
}

void
camera_set_rotation(struct camera *c, quaternion rotation)
{
	camera_set(c, c->position, rotation);
}

void
camera_set_projection(struct camera *c, float fov, float ratio)
{
	c->fov = fov;
	c->ratio = ratio;
	camera_update_proj(c);
}

void
camera_set_ratio(struct camera *c, float ratio)
{
	camera_set_projection(c, c->fov, ratio);
}

vec3
camera_get_left(struct camera *c)
{
	vec3 s;
	mat3 rot;
	quaternion_to_rot3(&rot, c->rotation);

	s.x = rot.m[0][0];
	s.y = rot.m[1][0];
	s.z = rot.m[2][0];

	return s;
}

vec3
camera_get_up(struct camera *c)
{
	vec3 u;
	mat3 rot;
	quaternion_to_rot3(&rot, c->rotation);

	u.x = rot.m[0][1];
	u.y = rot.m[1][1];
	u.z = rot.m[2][1];

	return u;
}

vec3
camera_get_dir(struct camera *c)
{
	vec3 f;
	mat3 rot;
	quaternion_to_rot3(&rot, c->rotation);

	f.x = rot.m[0][2];
	f.y = rot.m[1][2];
	f.z = rot.m[2][2];

	return f;
}

void
camera_move(struct camera *c, vec3 off)
{
	camera_set_position(c, vec3_add(c->position, off));
}

void
camera_apply(struct camera *c, quaternion q)
{
	c->rotation = quaternion_mult(q, c->rotation);
	camera_update_view(c);
}

void
camera_rotate(struct camera *c, vec3 axis, float angle)
{
	quaternion q = quaternion_axis_angle(axis, angle);
	camera_apply(c, q);
}

void
camera_look_at(struct camera *c, vec3 look_at, vec3 up)
{
	vec3 dir = vec3_normalize(vec3_sub(look_at, c->position));

	c->rotation = quaternion_look_at(dir, up);
	camera_update_view(c);
}

static void
camera_update_view(struct camera *c)
{
	float x, y, z;
	vec3 s, u, f;

	u = camera_get_up(c);
	f = camera_get_dir(c);
	f = vec3_normalize(f);
	u = vec3_normalize(u);
	s = vec3_cross(f, u);
	s = vec3_normalize(s);

/*
M = s0  s1  s2 0
    u0  u1  u2 0
   -f0 -f1 -f2 0
    0    0   0 1
*/
	/* changement de repère worldview to eyeview */
	x = -vec3_dot(s, c->position);
	y = -vec3_dot(u, c->position);
	z =  vec3_dot(f, c->position);
	{
		mat4 view = {{ /* tableau de vecteurs colonne */
			{  s.x,  u.x, -f.x, 0 },
			{  s.y,  u.y, -f.y, 0 },
			{  s.z,  u.z, -f.z, 0 },
			{    x,    y,    z, 1 }
			}};
		c->view = view;
	}
}

// https://github.com/g-truc/glm/blob/master/manual.md#section5_2
// https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/gluLookAt.xml
// https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/gluPerspective.xml

static void
camera_update_proj(struct camera *c)
{
	float tanHalfFov = tan(c->fov / 2.0);
	float f = c->zFar;
	float n = c->zNear;

	c->proj.m[0][0] = 1.0 / (c->ratio * tanHalfFov);
	c->proj.m[1][1] = 1.0 / (tanHalfFov);
	c->proj.m[2][2] = -(f + n) / (f - n);
	c->proj.m[2][3] = -1.0;
	c->proj.m[3][2] = (-2.0 * f * n) / (f - n);
	c->proj.m[3][3] = 0;
}

#if 0
static void
camera_update_proj(struct camera *c)
{
	float r = width;
	float t = height;
	float f = c->zFar;
	float n = c->zNear;

	c->proj.m[0][0] = 1.0 / r;
	c->proj.m[1][1] = 1.0 / t;
	c->proj.m[2][2] = -2.0  / (f - n);
	c->proj.m[2][3] = 0.0;
	c->proj.m[3][2] = -(f + n) / (f - n);
	c->proj.m[3][3] = 1.0;
}
#endif

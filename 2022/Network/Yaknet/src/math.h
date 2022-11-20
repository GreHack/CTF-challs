#pragma once

#include <math.h>

typedef struct vec2_t {
	float x;
	float y;
} vec2;

typedef struct vec3_t {
	float x;
	float y;
	float z;
} vec3;

typedef struct vec4_t {
	float x;
	float y;
	float z;
	float w;
} vec4;

typedef struct mat3_t {
	float m[3][3];
} mat3;

typedef struct mat4_t {
	float m[4][4];
} mat4;

typedef struct quaternion_t {
	vec3 v;
	float w;
} quaternion;

#define VEC3_ZERO   ((vec3){0, 0, 0})
#define VEC3_AXIS_X ((vec3){1, 0, 0})
#define VEC3_AXIS_Y ((vec3){0, 1, 0})
#define VEC3_AXIS_Z ((vec3){0, 0, 1})
#define MAT3_IDENTITY ((mat3){{{1, 0, 0}, {0, 1, 0}, {0, 0, 1}}})
#define MAT4_IDENTITY ((mat4){{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}}})
#define QUATERNION_IDENTITY ((quaternion){{0, 0, 0}, 1})

static inline vec2
vec2_neg(vec2 v)
{
	vec2 r = { -v.x, -v.y };
	return r;
}

static inline vec2
vec2_add(vec2 u, vec2 v)
{
	vec2 r = { u.x + v.x, u.y + v.y };
	return r;
}

static inline vec2
vec2_sub(vec2 u, vec2 v)
{
	vec2 r = { u.x - v.x, u.y - v.y };
	return r;
}

static inline float
vec2_dot(vec2 u, vec2 v)
{
	return u.x * v.x + u.y * v.y;
}

static inline vec2
vec2_mult(float s, vec2 v)
{
	vec2 r = { s * v.x, s * v.y };
	return r;
}

/* vec3 operations */
/* print_vec3
   Specification:
   Semantic: Take a vec3 v
   and print the vector v coordinate to the standard output
*/
void print_vec3(vec3 v);

/* vec3_add
   Specification:
   Takes two vec3 u, v and return a vec3
   Semantic: Performs the vectorial sum u + v and return a new vector w = u+v.
   Example: let u=(1,0,0) v=(0,1,0) be two vectors
   let w = u + v = (1,1,0)
*/
static inline vec3
vec3_add(vec3 u, vec3 v)
{
	vec3 r = { u.x + v.x, u.y + v.y, u.z + v.z };
	return r;
}

/* vec3_sub
   Specification:
   Takes two vec3 u, v and return a vec3
   Semantic: perform vectorial operation u - v and return a new vector w = u-v.
   Example: let u=(1,0,0) v=(0,1,0) be two vectors
   let w = u - v = (1,-1,0)
*/
static inline vec3
vec3_sub(vec3 u, vec3 v)
{
	vec3 r = { u.x - v.x, u.y - v.y, u.z - v.z };
	return r;
}

/* vec3_mult
   Specification:
   Takes a float s and a vec3 v and return a vec3
   Semantic: perform vector dilatation s*v and returns the vector w = s*v.
   Example: let s = 2.5  v=(0,1,0) a scalar and vector
   w = s * v = (0, 2.5, 0)
*/
static inline vec3
vec3_mult(float s, vec3 v)
{
	vec3 r = { s * v.x, s * v.y, s * v.z };
	return r;
}

/* vec3_neg
   Specification:
   Take a vec3 v and return a vec3
   Semantic: perform vector dilatation (-1)*v and returns the vector w = (-1)*v.
   Example: let v=(-5,1,2) be a vector
   w = (-1) * v = (5, -1, -2)
*/
static inline vec3
vec3_neg(vec3 v)
{
	vec3 r = { -v.x, -v.y, -v.z };
	return r;
}

/* vec3_dot
   Specification:
   Take two vec3 u and v and return a float
   Semantic: perform the dot product of u and v, returns the scalar w = u.v.
   let u=(u1, u2, u3)  v=(v1, v2, v3) two vectors
   w = u.v = u1*v1 + u2*v2 + u3*v3
   examples:
   - let u=(1,0,0) and v=(0,1,0) two (orthogonal) vectors
   w = u.v = 0
   - let u=(1,0,0) and v=(2,0,0) two (colinear) vectors
   w = u.v = 2
   - let u=(1,3,2) and v=(3,-1,2) two vectors
   w = 3-3+4 = 4
*/
static inline float
vec3_dot(vec3 u, vec3 v)
{
	return u.x * v.x + u.y * v.y + u.z * v.z;
}

/* vec3_norm
   Specification:
   Take a vec3 v and return a float
   Semantic: compute the norm(2) of the vector v returns the scalar w.
   let v=(v1, v2, v3)
   w = ||v|| = sqrt(v1^2 + v2^2 + v3^2) = sqrt(v.v)
   examples:
   - let v=(1,0,0)
   w = ||v|| = 1
   - let v=(1,1,0)
   w = ||v|| = sqrt(2)
   - let v=(1,3,2)
   w = sqrt(1+9+4) = sqrt(14)
*/
static inline float
vec3_norm(vec3 v)
{
	return sqrt(vec3_dot(v, v));
}


/* vec3_normalize
   Specification:
   Take a vec3 v and return a vec3
   Semantic: Perform vector dilatation w = 1/||v|| * v so that
   || w || = 1 and return the vector w.
   examples:
   - let v=(1,0,0)
   w = ||v|| = 1
   - let v=(1,1,0)
   w = 1/||v|| = 1/sqrt(2) * v = (1/sqrt(2), 1/sqrt(2), 0)
*/
static inline vec3
vec3_normalize(vec3 v)
{
	float n = vec3_norm(v);
	vec3 r = { v.x / n, v.y / n, v.z / n };
	return r;
}

static inline float
vec3_max(vec3 v)
{
	return MAX(v.x, MAX(v.y, v.z));
}

static inline float
mix(float a, float b, float x)
{
	return a * (1.0 - x) + b * x;
}

static inline vec3
vec3_lerp(vec3 u, vec3 v, float x)
{
	vec3 r = { mix(u.x, v.x, x), mix(u.y, v.y, x), mix(u.z, v.z, x) };
	return r;
}

static inline float
vec3_dist(vec3 u, vec3 v)
{
	return vec3_norm(vec3_sub(u, v));
}

static inline vec3
vec3_fma(vec3 u, vec3 v, vec3 w)
{
	vec3 r;
	r.x = u.x * v.x + w.x;
	r.y = u.y * v.y + w.y;
	r.z = u.z * v.z + w.z;
	return r;
}

/* vec3_cross
   Specification:
   Take two vec3 u and v and return a vec3
   Semantic: Perform vector cross product w = u x v and return the vector w.
   let u=(u1, u2, u3) and v=(v1, v2, v3)
   w = (u2*v3-u3*v2, u1*v3-u3*v1, u1*v2-u2*v1)
   with w othogonal to span(u,v).
   example:
   - let u=(0,1,0) and v=(1,0,0)
   w = (0,0,-1)
*/
static inline vec3
vec3_cross(vec3 u, vec3 v)
{
	vec3 r;
	r.x = u.y * v.z - u.z * v.y;
	r.y = u.z * v.x - u.x * v.z;
	r.z = u.x * v.y - u.y * v.x;
	return r;
}

/* print_vec4
   Specification:
   Semantic: Take a vec4 v
   and print the vector v coordinate to the standard output
*/
void print_vec4(vec4 v);

/* vec4_add
   Specification:
   Takes two vec4 u, v and return a vec4
   Semantic: Performs the vectorial sum u + v and return a new vector w = u+v.
   Example: let u=(1,0,0,1) v=(0,1,0,1) be two vectors
   let w = u + v = (1,1,0,2)
*/
static inline vec4
vec4_add(vec4 u, vec4 v)
{
	vec4 r = { u.x + v.x, u.y + v.y, u.z + v.z, u.w + v.w };
	return r;
}

/* vec4_sub
   Specification:
   Takes two vec4 u, v and return a vec4
   Semantic: perform vectorial operation u - v and return a new vector w = u-v.
   Example: let u=(1,0,0,1) v=(0,1,0,1) be two vectors
   let w = u - v = (1,-1,0,0)
*/
static inline vec4
vec4_sub(vec4 u, vec4 v)
{
	vec4 r = { u.x - v.x, u.y - v.y, u.z - v.z, u.w - v.w };
	return r;
}

/* vec4_mult
   Specification:
   Takes a float s and a vec4 v and return a vec4
   Semantic: perform vector dilatation s*v and returns the vector w = s*v.
   Example: let s = 2.5  v=(0,1,0,2) a scalar and vector
   w = s * v = (0, 2.5, 0, 5)
*/
static inline vec4
vec4_mult(float s, vec4 v)
{
	vec4 r = { s * v.x, s * v.y, s * v.z, s * v.w };
	return r;
}

/* vec4_neg
   Specification:
   Take a vec4 v and return a vec4
   Semantic: perform vector dilatation (-1)*v and returns the vector w = (-1)*v.
   Example: let v=(-5,1,2,1) be a vector
   w = (-1) * v = (5, -1, -2, -1)
*/
static inline vec4
vec4_neg(vec4 v)
{
	vec4 r = { -v.x, -v.y, -v.z, -v.w };
	return r;
}

/* vec4_dot
   Specification:
   Take two vec4 u and v and return a float
   Semantic: perform the dot product of u and v, returns the scalar w = u.v.
   let u=(u1, u2, u3, u4)  v=(v1, v2, v3, v4) two vectors
   w = u.v = u1*v1 + u2*v2 + u3*v3 + u4*v4
*/
static inline float
vec4_dot(vec4 u, vec4 v)
{
	return u.x * v.x + u.y * v.y + u.z * v.z + u.w * v.w;
}

/* vec4_norm
   Specification:
   Take a vec4 v and return a float
   Semantic: compute the norm(2) of the vector v returns the scalar w.
   let v=(v1, v2, v3, v4)
   w = ||v|| = sqrt(v1^2 + v2^2 + v3^2 + v4^2) = sqrt(v.v)
*/
static inline float
vec4_norm(vec4 v)
{
	return sqrt(vec4_dot(v, v));
}

/* vec4_normalize
   Specification:
   Take a vec4 v and return a vec4
   Semantic: Perform vector dilatation w = 1/||v|| * v so that
   || w || = 1 and return the vector w.
*/
static inline vec4
vec4_normalize(vec4 v)
{
	float n = vec4_norm(v);
	vec4 r = { v.x / n, v.y / n, v.z / n, v.w / n };
	return r;
}

/* mat3 operations */

/* print_vec3
   Semantic: Take a mat3 m and print the matrix to the standard output
*/
void print_mat3(mat3 m);

/* vec3_add
   Specification/Semantic:
   Return a mat3 identity matrix.
*/
mat3 mat3_id(void);

/* vec3_mult
   Specification:
   Takes a mat3 pointer m, and a float s and return a mat3
   Semantic: Performs the matrix dilatation s*m and return a new matrix p = s*m
   Example: let s=1.3 a scalar and m the 3x3 identity matrix
   p = s * m
     = (1.3, 0, 0)
       (0, 1.3, 0)
       (0, 0, 1.3)
*/
mat3 mat3_mult(mat3 *m, float s);

/* mat3_mult_vec3
   Specification:
   Takes a mat3 pointer m, and a vec3 v and return a vec3
   Semantic: Perform matrix-vector multiplication between M and v
   and return the resulting vector.
   Example:
   let M =  (0, -1, 0) the counter-clockwise 90° rotation around z axis.
            (1, 0, 0)
            (0, 0, 1)
   let v = (1,0,0)
   v' = M* v = (0,1,O)
*/

vec3 mat3_mult_vec3(mat3 *m, vec3 v);

/* mat3_mult_mat3
   Specification:
   Takes two mat3 pointers a, b and return a mat3
   Semantic: Perform matrix-matrix multiplication between a, b
   and return the resulting matrix.
*/
mat3 mat3_mult_mat3(mat3 *a, mat3 *b);

/* mat3_det
   Specification:
   Take a mat3 pointer m and return a float
   Semantic: Compute the determinant of the matrix M
*/
float mat3_det(mat3 *m);

/* mat3_invert
   Need to rewrite the implementation of this function.
*/
mat3 mat3_invert(mat3 *m);


/* mat4 */
void print_mat4(mat4 *m);
mat4 mat4_id(void);

/* mat4_transform
   Semantic: return the mat4 coresponding to the input
   rotation and translation
*/
mat4 mat4_transform(vec3 position, quaternion rotation);
mat4 mat4_transform_scale(vec3 position, quaternion rotation, vec3 scale);
mat4 mat4_mult(mat4 *m, float s);
vec4 mat4_mult_vec4(mat4 *m, vec4 v);
vec3 mat4_mult_vec3(mat4 *m, vec3 v);
mat4 mat4_mult_mat4(mat4 *a, mat4 *b);

/* mat4_projection_frustum
   Specification: Takes a projection matrix and return the frustum planes.
   Semantic: extract from a projection matrix 6 planes corresponsing
   to the projection frustum.
 */
void mat4_projection_frustum(mat4 *mv, vec4 planes[6]);


/* plane_signed_distance
   Specification: Takes a halfspace plane and a point and return the
   signed distance of the point to the plane.
 */
float plane_signed_distance(vec4 plane, vec3 point);

/* sphere_outside_frustum
   Specification: Take frustrum planes, a sphere center and it's radius
   return 1 if the sphere is outside of the frustum, ie the sphere is not
    visible from the camera frustum.
 */
int sphere_outside_frustum(vec4 planes[6], vec3 center, float radius);

/* Quaternions and Rotations */

/* print_quaternion
   Semantic: Take a quaternion q and print the quaternion coordinates
   to the standard output
*/
void print_quaternion(quaternion q);

/* quaternion_mult
   Specification: quaternion quaternion_mult(quaternion *a, quaternion *b)
   Semantic: Take two quaternions and return the multiplication of the two.
 */
quaternion quaternion_mult(quaternion a, quaternion b);
float quaternion_sqnorm(quaternion q);
float quaternion_norm(quaternion q);
quaternion quaternion_normalize(quaternion q);
quaternion quaternion_conjugate(quaternion q);
vec3 quaternion_rotate(quaternion q, vec3 v);

/* load_rot4
   Specification: void load_rot4(mat4 *d, vec3 *axis, float angle);
   Take a mat4 pointer, a vec3, a float
   Semantic: Given an axis a vector v and an angle alpha,
   load_rot4 initialize the 4x4 matrix representing the rotation
   of angle alpha around axis v in affine space.
   Pre-condition:
   The pointer d must point to the adress of a valid mat4 structure.
   Post-condition: The structure pointed by d contain
   the matrix representing the rotation of angle alpaha around v.
   Example:
*/
void load_rot4(mat4 *d, vec3 axis, float angle);

/* quaternion_from_rot3
   Specification:
   Take a mat3 pointer and return a quaternion
   Semantic: Given a rotation matrix M return the
   equivalent quaternion representation of the rotation.
   Pre-condition: M must be a rotation matrix (orthogonal, det=1, i.e M in SO3)
*/
quaternion quaternion_from_rot3(mat3 *m);
void quaternion_to_rot3(mat3 *d, quaternion q);
void quaternion_to_mat4(mat4 *d, quaternion q);

/* quaternion_axis_angle
   Specification:
   Take a vec3, a float and returns a quaternion
   Semantic: Given an axis vector v and an angle alpha,
   quaternion_axis_angle initialize and return a quaternion
   representing the rotation of angle alpha around axis v.
   Example:
*/
quaternion quaternion_axis_angle(vec3 axis, float angle);

/* quaternion_look_at
   Specification: quaternion quaternion_look_at(vec3 *dir, vec3 *up);
   Take two vec3 and returns a quaternion
   Semantic:
   Let's consider a camera placed in worldspace.
   A camera is associated to a orthonormal
   basis composed of:
   - f the forward vector defining the direction the camera is looking at.
   - s the side vector defining the tilt axis of the camera
   - u the up vector defining the pan axis of the camera

   Let's put ourselves in the point of view of the camera.
   The origin O is our eye.
   The camera is pointing forward f.
   The upward direction is defined by the u axis.
   The tilt axis is defined by s.
   We note <u,f> = span(u,f) the vertical plane of the camera.

   u

   ^
   |  f
   | /
   |/
   +------ > s
   O

   The quaternion lookat function compute the transformation
   that will point the forward axis f to the direction dir
   and make the new vertical plane of the camera be the <dir, up> plane.

   u
   ^    up
   |   .
   |
   |  .  f
   |    /
   | . /   .'dir
   |  /  .'
   |./ .'
   |/.'
   +---------------- > s
   O.
     .
      .
       .
        x'

   Pre-condition:
   The up and dir vectors must be non-colinear so they can define the vertical
   plane of the camera after the rotation
*/
quaternion quaternion_look_at(vec3 dir, vec3 up);

float ray_distance_to_plane(vec3 org, vec3 dir, vec4 plane);
int point_in_triangle(vec3 q, vec3 a, vec3 b, vec3 c);


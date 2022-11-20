#pragma once
#include "math.h"
struct camera {
	vec3 position;
	quaternion rotation;
	float ratio, fov, zNear, zFar;

	mat4 proj, view;
};

void camera_init(struct camera *c, float fov, float ratio);

void camera_set(struct camera *c, vec3 pos, quaternion rotation);
void camera_set_znear(struct camera *c, float z);
void camera_set_position(struct camera* c, vec3 pos);
void camera_set_rotation(struct camera *c, quaternion q);
void camera_set_projection(struct camera* c, float fov, float ratio);
void camera_set_ratio(struct camera* c, float ratio);

vec3 camera_get_up(struct camera* c);
vec3 camera_get_dir(struct camera* c);
vec3 camera_get_left(struct camera* c);

void camera_move(struct camera *c, vec3 off);
void camera_apply(struct camera *c, quaternion q);
void camera_rotate(struct camera *c, vec3 axis, float angle);

void camera_look_at(struct camera *c, vec3 look_at, vec3 up);


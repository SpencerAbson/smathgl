#ifndef SMATH_CAMERA_H_
#define SMATH_CAMERA_H_
#include <math.h>
#include <stdbool.h>
#include "matrices.h"
#include "vectors.h"

// camera defualts
#define CAM_DEFAULT_YAW -90.0f
#define CAM_DEFAULT_PITCH 0.0f
#define CAM_DEFAULT_SPEED 2.5f
#define CAM_DEFAULT_SENS 0.1f
#define CAM_DEFAULT_ZOOM 45.0f

enum Sm_CameraDirection
{
FORWARD,
BACKWARD,
LEFT,
RIGHT,
UP,
DOWN
}CameraDirection;

typedef struct SmCamera_t
{
    vec3 position;
    vec3 front;
    vec3 right;
    vec3 up;
    vec3 world_up;

    float pitch;
    float yaw;
    float movement_speed;
    float mouse_sensitivity;
    float zoom;

}SmCamera;

SmCamera *cam_create(vec3 position, vec3 up, float yaw, float pitch);
void cam_process_keyboard(SmCamera *self, enum Sm_CameraDirection directiom, float delta_time);
void cam_process_mouse(SmCamera *self, float x_offset, float y_offset, bool pitch_constraint);
void cam_process_scroll(SmCamera *self, float y_offset);
void cam_lookat(SmCamera *self, mat4x4 out);

#endif // CAMERA_H_

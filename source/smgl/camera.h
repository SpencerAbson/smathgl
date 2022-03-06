#ifndef SMATH_CAMERA_H_
#define SMATH_CAMERA_H_
#include <math.h>
#include <stdbool.h>
#include "matrices.h"
#include "vectorf.h"

// camera defualts
#define CAM_DEFAULT_YAW -90.0f
#define CAM_DEFAULT_PITCH 0.0f
#define CAM_DEFAULT_SPEED 2.5f
#define CAM_DEFAULT_SENS 0.1f
#define CAM_DEFAULT_ZOOM 45.0f

enum SmCameraDirection
{
CAMDIR_FORWARD,
CAMDIR_BACKWARD,
CAMDIR_LEFT,
CAMDIR_RIGHT,
CAMDIR_UP,
CAMDIR_DOWN
}CameraDirection;

typedef struct SmCamera_t
{
    fvec position;
    fvec front;
    fvec right;
    fvec up;
    fvec world_up;

    float pitch;
    float yaw;
    float movement_speed;
    float mouse_sensitivity;
    float zoom;

}SmCamera;

SmCamera *cam_create(fvec position, fvec up, float yaw, float pitch);
void cam_process_keyboard(SmCamera *self, enum SmCameraDirection directiom, float delta_time);
void cam_process_mouse(SmCamera *self, float x_offset, float y_offset, bool pitch_constraint);
void cam_process_scroll(SmCamera *self, float y_offset);
mat4x4 cam_lookat(SmCamera *self);

#endif // CAMERA_H_

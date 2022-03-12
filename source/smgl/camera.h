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

typedef struct sm_camera
{
    fvec_t position;
    fvec_t front;
    fvec_t right;
    fvec_t up;
    fvec_t world_up;

    float pitch;
    float yaw;
    float movement_speed;
    float mouse_sensitivity;
    float zoom;

}sm_camera_t;

extern sm_camera_t *sm_cam_create(fvec_t position, fvec_t up, float yaw, float pitch);
extern void sm_cam_process_keyboard(sm_camera_t *self, enum SmCameraDirection directiom, float delta_time);
extern void sm_cam_process_mouse(sm_camera_t *self, float x_offset, float y_offset, bool pitch_constraint);
extern void sm_cam_process_scroll(sm_camera_t *self, float y_offset);
extern void sm_cam_lookat(mat4_t *out, sm_camera_t const *self);

#endif // CAMERA_H_

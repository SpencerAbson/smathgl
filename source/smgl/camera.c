#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <string.h>
#include "..\..\include/platform.h"
#include "..\..\include/smathgl.h"
#include "matrix_vector_op.h"
#include "camera.h"

/* Example of how one might implement an FPS camera using the basics of the lib. */

static void sm_update_camera_vectors(sm_camera_t *self)
{
    fvec_t new_front;
    fvec3_init(new_front, cosf(SMGL_RADIANS(self->yaw)) * cosf(SMGL_RADIANS(self->pitch)),
                              sinf(SMGL_RADIANS(self->pitch)),
                              sinf(SMGL_RADIANS(self->yaw)) * cosf(SMGL_RADIANS(self->pitch)));

    fvec_normalize(self->front, new_front);
    fvec_cross(self->right, self->front, self->world_up);
    fvec_normalize(self->right, self->right);
    fvec_cross(self->up, self->right, self->front);
    fvec_normalize(self->up, self->up);
}


sm_camera_t *sm_cam_create(fvec_t position, fvec_t up, float yaw, float pitch)
{
    assert(position.size == 3 && up.size == 3); // FIXME: 0 magnitude up will lead to normalization error, consider catching this.
    sm_camera_t *self = malloc(sizeof(sm_camera_t));
    self->position = position;
    self->world_up = up;
    self->yaw      = yaw;
    self->pitch    = pitch;

    fvec3_init(self->front, 0.0f, 0.0f, -1.0f);
    self->movement_speed    = CAM_DEFAULT_SPEED;
    self->mouse_sensitivity = CAM_DEFAULT_SENS;
    self->zoom              = CAM_DEFAULT_ZOOM;

    sm_update_camera_vectors(self);
    return self;
}


void sm_cam_process_keyboard(sm_camera_t *self, enum SmCameraDirection direction, float delta_time)
{
    float velocity = self->movement_speed * delta_time; // displacement ?
    fvec_t intermediate;

    switch(direction)
    {
        case CAMDIR_FORWARD:
            fvec_scale(&intermediate, &self->front, velocity);
            fvec_add(self->position, self->position, intermediate);
            break;

        case CAMDIR_BACKWARD:
            fvec_scale(&intermediate, &self->front, velocity);
            fvec_sub(self->position, self->position, intermediate);
            break;

        case CAMDIR_LEFT:
            fvec_scale(&intermediate, &self->right, velocity);
            fvec_sub(self->position, self->position, intermediate);
            break;

        case CAMDIR_RIGHT:
            fvec_scale(&intermediate, &self->right, velocity);
            fvec_add(self->position, self->position, intermediate);
            break;

        case CAMDIR_UP:
            fvec_scale(&intermediate, &self->up, velocity);
            fvec_add(self->position, self->position, intermediate);
            break;

        case CAMDIR_DOWN:
            fvec_scale(&intermediate, &self->up, velocity);
            fvec_sub(self->position, self->position, intermediate);
    }

}


void sm_cam_process_mouse(sm_camera_t *self, float x_offset, float y_offset, bool pitch_constraint)
{
    x_offset *= self->mouse_sensitivity;
    y_offset *= self->mouse_sensitivity;

    self->yaw   += x_offset;
    self->pitch += y_offset;

    if(pitch_constraint)
    {
        if(self->pitch > 89.0f) self->pitch = 89.0f;
        if(self->pitch < -89.0f) self->pitch = -89.0f;
    }

    sm_update_camera_vectors(self);
}


void sm_cam_process_scroll(sm_camera_t *self, float y_offset)
{
    self->zoom -= y_offset;
    if(self->zoom < 1.0f) self->zoom = 1.0f;
    if(self->zoom > 45.0f) self->zoom = 45.0f;
}


void sm_cam_lookat(mat4_t *output, sm_camera_t const *self)
{
    fvec_t pos_front;
    fvec_add(pos_front, self->position, self->front);
    mat4_lookat(output, &self->position, &pos_front, &self->up);
}

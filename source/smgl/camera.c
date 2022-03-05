#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <string.h>
#include "..\..\include/platform.h"
#include "..\..\include/smathgl.h"
#include "camera.h"


void update_camera_vectors(SmCamera *self)
{
    fvec new_front;
    new_front.data.values[0] = cosf(RADIANS(self->yaw)) * cosf(RADIANS(self->pitch));
    new_front.data.values[1] = sinf(RADIANS(self->pitch));
    new_front.data.values[2] = sinf(RADIANS(self->yaw)) * cosf(RADIANS(self->pitch)); // from sinf

    new_front   = fvec_normalize(&new_front);
    self->front = new_front;

    self->right = fvec_cross(&self->front, &self->world_up);
    self->right = fvec_normalize(&self->right);
    self->up    = fvec_cross(&self->right, &self->front);
    self->up    = fvec_normalize(&self->up);
}


SmCamera *cam_create(fvec position, fvec up, float yaw, float pitch)
{
    assert(position.size == 4 && up.size == 3);
    SmCamera *self = malloc(sizeof(SmCamera));
    self->position = position;
    self->up       = up;
    self->yaw      = yaw;
    self->pitch    = pitch;

    self->front.data.values[0] = 0.0f;
    self->front.data.values[1] = 0.0f;
    self->front.data.values[2] = -1.0f;
    self->movement_speed    = CAM_DEFAULT_SPEED;
    self->mouse_sensitivity = CAM_DEFAULT_SENS;
    self->zoom              = CAM_DEFAULT_ZOOM;

    update_camera_vectors(self);
    return self;
}


void cam_process_keyboard(SmCamera *self, enum Sm_CameraDirection direction, float delta_time)
{
    float velocity = self->movement_speed * delta_time; // displacement ?
    fvec intermediate;

    switch(direction)
    {
        case FORWARD:
            intermediate   = fvec_scale(&self->front, velocity);
            self->position = fvec_add(&self->position, &intermediate);
            break;

        case BACKWARD:
            intermediate   = fvec_scale(&self->front, velocity);
            self->position = fvec_sub(&self->position, &intermediate);
            break;

        case LEFT:
            intermediate   = fvec_scale(&self->right, velocity);
            self->position = fvec_sub(&self->position, &intermediate);
            break;

        case RIGHT:
            intermediate   = fvec_scale(&self->right, velocity);
            self->position = fvec_add(&self->position, &intermediate);
            break;

        case UP:
            intermediate   = fvec_scale(&self->up, velocity);
            self->position = fvec_add(&self->position, &intermediate);
            break;

        case DOWN:
            intermediate   = fvec_scale(&self->up, velocity);
            self->position = fvec_sub(&self->position, &intermediate);
    }

}


void cam_process_mouse(SmCamera *self, float x_offset, float y_offset, bool pitch_constraint)
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

    update_camera_vectors(self);
}


void cam_process_scroll(SmCamera *self, float y_offset)
{
    self->zoom -= y_offset;
    if(self->zoom < 1.0f) self->zoom = 1.0f;
    if(self->zoom > 45.0f) self->zoom = 45.0f;
}


void cam_lookat(SmCamera *self, mat4x4 out)
{
    fvec pos_front;
    pos_front = fvec_add(&self->position, &self->front);
    set_lookat(&self->position, &pos_front, &self->up, out);
}

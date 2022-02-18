#include <stdlib.h>
#include <math.h>
#include "..\include/smathgl.h"
#include "..\include/camera.h"


void update_camera_vectors(SmCamera *self)
{
    vec3 new_front;
    new_front[0] = cosf(RADIANS(self->yaw)) * cosf(RADIANS(self->pitch));
    new_front[1] = sinf(RADIANS(self->pitch));
    new_front[2] = sinf(RADIANS(self->yaw)) * cosf(RADIANS(self->pitch)); // from sinf

    vec_normalize(new_front, 3, new_front);
    memcpy(self->front, new_front, 3 * sizeof(float));

    vec_cross(self->front, self->world_up, self->right);
    vec_normalize(self->right, 3, self->right);
    vec_cross(self->right, self->front, self->up);
    vec_normalize(self->up, 3, self->up);
}


SmCamera *cam_create(vec3 position, vec3 up, float yaw, float pitch)
{
    SmCamera *self = malloc(sizeof(SmCamera));
    memcpy(self->position, position, sizeof(vec3));
    memcpy(self->world_up, up, sizeof(vec3));
    self->yaw      = yaw;
    self->pitch    = pitch;

    self->front[0] = 0.0f;
    self->front[1] = 0.0f;
    self->front[2] = -1.0f;
    self->movement_speed    = CAM_DEFAULT_SPEED;
    self->mouse_sensitivity = CAM_DEFAULT_SENS;
    self->zoom              = CAM_DEFAULT_ZOOM;

    update_camera_vectors(self);
    return self;
}


void cam_process_keyboard(SmCamera *self, enum Sm_CameraDirection direction, float delta_time)
{
    float velocity = self->movement_speed * delta_time; // displacement ?
    vec3 intermediate;

    switch(direction)
    {
        case FORWARD:
            vec_scale(self->front, velocity, intermediate);
            vec_add(self->position, intermediate, self->position);
            break;

        case BACKWARD:
            vec_scale(self->front, velocity, intermediate);
            vec_sub(self->position, intermediate, self->position);
            break;

        case LEFT:
            vec_scale(self->right, velocity, intermediate);
            vec_sub(self->position, intermediate, self->position);
            break;

        case RIGHT:
            vec_scale(self->right, velocity, intermediate);
            vec_add(self->position, intermediate, self->position);
            break;

        case UP:
            vec_scale(self->up, velocity, intermediate);
            vec_add(self->position, intermediate, self->position);
            break;

        case DOWN:
            vec_scale(self->up, velocity, intermediate);
            vec_sub(self->position, intermediate, self->position);
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
    vec3 pos_front;
    vec_add(self->position, self->front, pos_front);
    set_lookat(self->position, pos_front, self->up, out);
}

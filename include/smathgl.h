#ifndef SMATHGL_H_
#define SMATHGL_H_

#include <math.h>
#include "..\source/smgl/vectorf.h"
#include "..\source/smgl/matrices.h"

/* general use */
fvec mult_point_matrix(fvec *in, mat4x4 mat);
/* Euler rotation  */
//extern mat4x4 mat4_rotate(const mat4x4 *in, float angle, fvec *unit_vector);
//extern mat4x4 mat4_rotation(float angle, fvec *unit_vector);

/* lookat and perspective matrices  */
//extern mat4x4 mat4_perspective(float angle_rad, float aspect_ratio, float near, float far);
//extern mat4x4 mat4_lookat(fvec *position, fvec *target, fvec *up);

#endif // SMATHGL_H_

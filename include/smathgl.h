#ifndef SMATHGL_H_
#define SMATHGL_H_

#include <math.h>
#include "..\source/smgl/vectorf.h"
#include "..\source/smgl/matrices.h"

/* general use */
fvec mult_point_matrix(fvec *in, mat4x4 mat);
/* Euler rotation  */
void rotate4x4(mat4x4 in, float angle, fvec *unit_vector, mat4x4 out);
void set_rotation4x4(mat4x4 in, float angle, fvec *unit_vector);

/* lookat and perspective matrices  */
void set_perspective(float angle_rad, float aspect_ratio, float near, float far, mat4x4 out);
void set_lookat(fvec *position, fvec *target, fvec *up, mat4x4 out);

#endif // SMATHGL_H_

#ifndef SMATHGL_H_
#define SMATHGL_H_

#include <math.h>
#include "..\source/smgl/vectorf.h"
//#include "..\source/quaternions.h"
#include "..\source/smgl/matrices.h"

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

#define RADIANS(x) (x/180.0f * (float)M_PI)

/* general use */
void mult_point_matrix(const vec3 in, mat4x4 mat, vec3 out);
/* Euler rotation  */
void rotate4x4(mat4x4 in, float angle, vec3 unit_vector, mat4x4 out);
void set_rotation4x4(mat4x4 in, float angle, vec3 unit_vector);

/* lookat and perspective matrices  */
void set_perspective(float angle_rad, float aspect_ratio, float near, float far, mat4x4 out);
void set_lookat(const vec3 position, const vec3 target, const vec3 up, mat4x4 out);

#endif // SMATHGL_H_

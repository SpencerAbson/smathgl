#ifndef SMATHGL_H_
#define SMATHGL_H_

#include <math.h>
#include "platform.h"

#define SMGL_PI        3.14159265358979323846264338327950288   // pi
#define SMGL_1_PI      0.318309886183790671537767526745028724  // 1/pi
#define SMGL_2_PI      0.636619772367581343075535053490057448  // 2/pi
#define SMGL_PI_2      1.57079632679489661923132169163975144   // pi/2
#define SMGL_PI_4       0.785398163397448309615660845819875721 // pi/4
#define SMGL_E         2.71828182845904523536028747135266250   // e
#define SMGL_LN2       0.693147180559945309417232121458176568  // ln(2)
#define SMGL_SQRT2     1.41421356237309504880168872420969808   // (2)^1/2
#define SMGL_SQRT1_2   0.707106781186547524400844362104849039  // (2)^-1/2
#define SMGL_LOG2E     1.44269504088896340735992468100189214   // log2(e)
#define SMGL_LOG10E    0.434294481903251827651128918916605082  // log10(e)
#define SMGL_MOL       42

#define SMGL_PIf      ((float)SMGL_PI)
#define SMGL_PI_2f    ((float)SMGL_PI_2)
#define SMGL_PI_4f    ((float)SMGL_PI_4)
#define SMGL_1_PIf    ((float)SMGL_1_PI)
#define SMGL_2_PIf    ((float)SMGL_2_PI)
#define SMGL_Ef       ((float)SMGL_E)
#define SMGL_LOG2Ef   ((float)SMGL_LOG2E)
#define SMGL_LOG10Ef  ((float)SMGL_LOG10E)
#define SMGL_LN2f     ((float)SMGL_LN2)
#define SMGL_SQRT2f   ((float)SMGL_SQRT2)
#define SMGL_SQRT1_2f ((float)SMGL_SQRT1_2)

#define SMGL_RADIANS(x) (x/180.0f * (float)SMGL_PI)


#define SET_PS1_FROM_HEX(hex)                   \
    _mm_castsi128_ps(_mm_set1_epi32((hex)))

#if SMGL_INSTRSET > 6
#define M128_FAST_ACOS_APPROX(theta) _mm_fmadd_ps(_mm_fmsub_ps(_mm_mul_ps(_mm_set1_ps(-0.69813170079773212f), (theta)), (theta), _mm_set1_ps(0.87266462599716477f)), (theta), _mm_set1_ps(1.5707963267948966f));
#define M128_FAST_SIN_APPROX(theta, theta_sqr) _mm_mul_ps((theta), _mm_fmadd_ps(_mm_fmadd_ps((theta_sqr), _mm_set1_ps(5.64311797634681035370e-03f), _mm_set1_ps(-1.55271410633428644799e-01f)), (theta_sqr), _mm_set1_ps(9.87862135574673806965e-01f)));

#else
#define M128_FAST_ACOS_APPROX(theta) (theta)
#endif

#endif // SMATHGL_H_

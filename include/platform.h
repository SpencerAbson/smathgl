#ifndef SMGL_PLATFORM_H_
#define SMGL_PLATFORM_H_

#if (defined(_M_AMD64) || defined(_M_X64) || defined(__amd64) ) && ! defined(__x86_64__)
#define __x86_64__ 1
#endif

/* floating point vectors must be aligned in 16 byte boundary for SIMD */
#if defined( _MSC_VER )
#define SMGL_FORCEINLINE __forceinline
#define SMGL_ALIGN_16 __declspec(align(16))
#define SMGL_ALIGN_32 __declspec(align(32))
#elif defined( __GNUC__ ) || defined (__clang__)
#define SMGL_FORCEINLINE __attribute__((always_inline))
#define SMGL_ALIGN_16 __attribute__((aligned(16)))
#define SMGL_ALIGN_32 __attribute__((aligned(32)))
#else
#error "Unsupported compiler (smgl)"
#endif

/* selection of instruction set level */
#ifndef SMGL_INSTRSET // INSTRUCTION SET
#if defined ( __AVX512VL__ ) && defined ( __AVX512BW__ ) && defined ( __AVX512DQ__ )
#define SMGL_INSTRSET 10
#elif defined ( __AVX512F__ ) || defined ( __AVX512__ )
#define SMGL_INSTRSET 9
#elif defined ( __AVX2__ )
#define SMGL_INSTRSET 8
#elif defined ( __AVX__ )
#define SMGL_INSTRSET 7
#elif defined ( __SSE4_2__ )
#define SMGL_INSTRSET 6
#elif defined ( __SSE4_1__ )
#define SMGL_INSTRSET 5
#elif defined ( __SSSE3__ )
#define SMGL_INSTRSET 4
#elif defined ( __SSE3__ )
#define SMGL_INSTRSET 3
#elif defined ( __SSE2__ ) || defined ( __x86_64__ )
#define SMGL_INSTRSET 2
#elif defined ( __SSE__ )
#define SMGL_INSTRSET 1
#elif defined ( _M_IX86_FP )           // Defined in MS compiler. 1: SSE, 2: SSE2
#define SMGL_INSTRSET _M_IX86_FP
#else
#error "Unable to compile due to unsupported instruction set (smgl)."
#endif // instruction set defines
#endif // SMGL_INSTRSET

#if SMGL_INSTRSET > 7                       // AVX2 and later
#if defined (__GNUC__) && ! defined (__INTEL_COMPILER)
#include <x86intrin.h>
#else
#include <immintrin.h>                 // MS/Intel version of immintrin.h covers AVX and later
#endif // __GNUC__
#elif SMGL_INSTRSET == 7
#include <immintrin.h>                 // AVX
#elif SMGL_INSTRSET == 6
#include <nmmintrin.h>                 // SSE4.2
#elif SMGL_INSTRSET == 5
#include <smmintrin.h>                 // SSE4.1
#elif SMGL_INSTRSET == 4
#include <tmmintrin.h>                 // SSSE3
#elif SMGL_INSTRSET == 3
#include <pmmintrin.h>                 // SSE3
#elif SMGL_INSTRSET == 2
#include <emmintrin.h>                 // SSE2
#elif SMGL_INSTRSET == 1
#include <xmmintrin.h>                 // SSE
#endif // INSTRSET


// AMD  instruction sets
#if defined (__XOP__) || defined (__FMA4__)
#ifdef __GNUC__
#include <x86intrin.h>                 // AMD XOP (Gnu)
#else
#include <ammintrin.h>                 // AMD XOP (Microsoft)
#endif //  __GNUC__
#elif defined (__SSE4A__)              // AMD SSE4A
#include <ammintrin.h>
#endif


#endif // SMGL_PLATFORM_H_

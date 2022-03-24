# cmake script for adding submodule as subdirectory
cmake_minimum_required(VERSION 3.13)
project(smgl VERSION 1.0 DESCRIPTION "SIMD 3D Maths Library")

add_library(
  smgl
  source/smgl/camera.c
  source/smgl/matrix_vector_op.c
  source/smgl/matrices.c
  source/smgl/quaternions.c
  source/smgl/vectori.c
  )

IF(CMAKE_BUILD_TYPE EQUAL "Debug")
  add_compile_definitions(SMGL_DEBUG)

IF(MSVC)
  add_definitions(/arch:AVX2)
  add_definitions(/O2)
ELSE(MSVC)
  add_definitions(-msse4.2)
  add_definitions(-O2)
  add_definitions(march=native)
ENDIF(MSVC)

target_include_directories(smgl PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/source")

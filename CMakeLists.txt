# cmake script for adding submodule as subdirectory
cmake_minimum_required(VERSION 3.13)
project(smgl VERSION 1.0 DESCRIPTION "SIMD 3D Maths Library")

add_library(
  smgl
  source/vectorf.c
  source/camera.c
  source/smathgl.c
  source/matrices.c
  source/quaternions.c
  )


IF(MSVC)
  add_definitions(/arch:AVX2)
  add_definitions(/O2)
ELSE(MSVC)
  add_definitions(-O3)
  add_definitions(-msse4.2)
ENDIF(MSVC)

target_include_directories(smgl PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/source")

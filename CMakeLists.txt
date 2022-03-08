# cmake script for adding submodule as subdirectory
cmake_minimum_required(VERSION 3.13)
project(smgl VERSION 1.0 DESCRIPTION "SIMD 3D Maths Library")

add_library(
  smgl
  source/smgl/vectorf.c
  source/smgl/camera.c
  source/smgl/smathgl.c
  source/smgl/matrices.c
  source/smgl/quaternions.c
  )


IF(MSVC)
  add_definitions(/arch:AVX2)
  IF(CMAKE_BUILD_TYPE EQUAL "RELEASE")
    add_definitions(/O2)
  ENDIF()
ELSE(MSVC)
  add_definitions(-O3)
  add_definitions(-msse4.2)
ENDIF(MSVC)

target_include_directories(smgl PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/source")

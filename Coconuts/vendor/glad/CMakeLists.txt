cmake_minimum_required(VERSION 3.1)
project(GLAD VERSION 0.1.33 LANGUAGES C)

add_library(glad src/glad.c)
target_include_directories(glad PRIVATE "${PROJECT_SOURCE_DIR}/include")

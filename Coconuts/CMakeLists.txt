cmake_minimum_required(VERSION 3.1)
project(COCONUTS VERSION 0.1 LANGUAGES CXX)

set (CMAKE_CXX_STANDARD 11)

#option(BUILD_SHARED_LIBS "Build shared libraries" OFF)


set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(vendor/glfw/)

add_subdirectory(vendor/spdlog/)

add_subdirectory(src)
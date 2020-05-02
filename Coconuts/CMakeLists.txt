cmake_minimum_required(VERSION 3.1)
project(COCONUTS VERSION 0.1 LANGUAGES CXX)

set (CMAKE_CXX_STANDARD 11)

option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
add_subdirectory(src)
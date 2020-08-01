cmake_minimum_required(VERSION 3.1)
project(COCONUTS VERSION 0.1 LANGUAGES CXX)

set (CMAKE_CXX_STANDARD 11)

# Vendor
# -------------------------------------------------------------------------------------------
#GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(vendor/glfw/)
set_target_properties(glfw PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib")

#GLAD
add_subdirectory(vendor/glad/)
set_target_properties(glad PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib")

#SPDLOG
add_subdirectory(vendor/spdlog/)
# -------------------------------------------------------------------------------------------


# Preprocessor macros
#Makes the GLFW header not include any OpenGL or OpenGL ES API header (done by GLAD)
add_definitions(-DGLFW_INCLUDE_NONE)

#Needed before including stb_image.h
add_definitions(-DSTB_IMAGE_IMPLEMENTATION)


# Coconuts source
add_subdirectory(src/core)
add_subdirectory(src/editor)


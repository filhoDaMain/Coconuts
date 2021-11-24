cmake_minimum_required(VERSION 3.1)
project(COCONUTS VERSION 0.1 LANGUAGES CXX)

set (CMAKE_CXX_STANDARD 11)

# Enable Profiling
set(COCONUTS_DEBUGGING_ENABLE ON CACHE BOOL "" FORCE)

# Look for hard dependencies
# For GNU-Linux build xorg-dev has to be installed
if(UNIX AND NOT APPLE)
    find_package(X11)
    if(${X11_FOUND})
        message(status "Found X11")
    else()
        string(ASCII 27 Esc)
        set(ColorReset "${Esc}[m")
        set(BoldRed     "${Esc}[1;31m")
        set(BoldGreen   "${Esc}[1;32m")
        set(BoldYellow  "${Esc}[1;33m")

        # stop build
        message(FATAL_ERROR "${BoldRed}\nDEPENDENCY NOT FOUND!\n* X11 'xorg-dev' package not found\n${BoldYellow}HINT: Install 'xorg-dev' and comeback later to build Coconuts\n${ColorReset}debian: sudo apt-get install -y xorg-dev")
        
    endif()
endif()

# Create macro for Build Tree root directory
add_definitions(-DCOCONUTS_BUILDTREE_ROOTDIR="${PROJECT_SOURCE_DIR}")

# Debug Macros
if (COCONUTS_DEBUGGING_ENABLE)
    add_definitions(-DCOCONUTS_DEBUG_TIMEPROFILER)
endif()

# Set Default Shaders location
set(DEFAULT_SHADER_SRCDIR "${PROJECT_SOURCE_DIR}/include/coconuts/graphics/shaders")

# Set path for build byproducts directory
set(BUILD_BYPRODUCTS_DIR "${CMAKE_BINARY_DIR}/build_byproducts")

# Create build artifacts directory
file(MAKE_DIRECTORY "${BUILD_BYPRODUCTS_DIR}")


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

#ENTITYX
set(ENTITYX_BUILD_TESTING false CACHE BOOL "Enable building of tests." FORCE)
set(ENTITYX_BUILD_SHARED false CACHE BOOL "Build shared libraries?" FORCE)
add_subdirectory(vendor/entityx/)
set_target_properties(entityx PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib")

#YAML-CPP
set(YAML_CPP_BUILD_TOOLS false CACHE BOOL "Enable parse tools" FORCE)
add_subdirectory(vendor/yaml-cpp/)
set_target_properties(yaml-cpp PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib")
# -------------------------------------------------------------------------------------------


# Preprocessor macros
#Makes the GLFW header not include any OpenGL or OpenGL ES API header (done by GLAD)
add_definitions(-DGLFW_INCLUDE_NONE)

#Needed before including stb_image.h
add_definitions(-DSTB_IMAGE_IMPLEMENTATION)

# Coconuts source
add_subdirectory(src/core)
add_subdirectory(src/editor/library)
add_subdirectory(src/editor/standalone)

# NOTE:
#
#   This CMakeLists.txt does not belong to the original nativefiledialog
#   project.
#
#   It was added in order to facilitate the nativefiledialog source code
#   integration.
#
#
#   The nativefiledialog directory within this directory is the pristine
#   nativefiledialog repo.
#
#



cmake_minimum_required(VERSION 3.1...3.17 FATAL_ERROR)

project(NFD VERSION 1.1.6 LANGUAGES C)

# Platfom files
# macOS / OSX
if (APPLE)
    add_library(nfd nativefiledialog/src/nfd_common.c
                    nativefiledialog/src/nfd_cocoa.m)

# GNU:
elseif(UNIX AND NOT APPLE)
    add_library(nfd nativefiledialog/src/nfd_common.c
                    nativefiledialog/src/nfd_gtk.c)
endif()

# Includes
target_include_directories(nfd PUBLIC ${PROJECT_SOURCE_DIR}/nativefiledialog/src/
                                      ${PROJECT_SOURCE_DIR}/nativefiledialog/src/include/)
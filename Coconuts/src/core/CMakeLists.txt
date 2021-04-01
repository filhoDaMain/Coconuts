# CORE LIBRARY

add_library(ccncore core.cpp)

add_subdirectory(application)
add_subdirectory(logger)
add_subdirectory(event_system)
add_subdirectory(layer_system)
add_subdirectory(cameras)
add_subdirectory(graphics)
add_subdirectory(ecs)
add_subdirectory(asset_manager)


# Platform Specific files
# Mac OS
if (APPLE)
    message(STATUS "MacOS Platform detected")
    message(STATUS "Using OpenGL Graphics API")
    add_subdirectory(platform/MacOS)
    add_subdirectory(platform/OpenGL)
elseif(UNIX AND NOT APPLE)
    message(STATUS "GNU Platform detected")
    message(STATUS "Using OpenGL Graphics API")
    add_subdirectory(platform/GNU)
    add_subdirectory(platform/OpenGL)
endif()

target_include_directories(ccncore PRIVATE "${PROJECT_SOURCE_DIR}/include"
                                        "${PROJECT_SOURCE_DIR}/vendor/spdlog/include"
                                        "${PROJECT_SOURCE_DIR}/vendor/glfw/include"
                                        "${PROJECT_SOURCE_DIR}/vendor/glad/include"
                                        "${PROJECT_SOURCE_DIR}/vendor/glm"
                                        "${PROJECT_SOURCE_DIR}/vendor/stb"
                                        "${PROJECT_SOURCE_DIR}/vendor/entityx")

set_target_properties(ccncore PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib")


# Copy 3rd party header files meant to be available to client
ADD_CUSTOM_COMMAND(TARGET ccncore POST_BUILD
                   
                   # spdlog
                   COMMAND rm -rf "${PROJECT_SOURCE_DIR}/include/vendor/spdlog/*"
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       "${PROJECT_SOURCE_DIR}/vendor/spdlog/include"
                       "${PROJECT_SOURCE_DIR}/include/vendor"

                   # glm
                   COMMAND rm -rf "${PROJECT_SOURCE_DIR}/include/vendor/glm/*"
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       "${PROJECT_SOURCE_DIR}/vendor/glm/glm"
                       "${PROJECT_SOURCE_DIR}/include/vendor/glm"

                   # entityx (partitioned in severeal header files copies)
                   COMMAND rm -rf "${PROJECT_SOURCE_DIR}/include/vendor/entityx/*"
                   COMMAND ${CMAKE_COMMAND} -E make_directory "${PROJECT_SOURCE_DIR}/include/vendor/entityx"
                   COMMAND ${CMAKE_COMMAND} -E make_directory "${PROJECT_SOURCE_DIR}/include/vendor/entityx/3rdparty"
                   COMMAND ${CMAKE_COMMAND} -E make_directory "${PROJECT_SOURCE_DIR}/include/vendor/entityx/deps"
                   COMMAND ${CMAKE_COMMAND} -E make_directory "${PROJECT_SOURCE_DIR}/include/vendor/entityx/help"
                   COMMAND ${CMAKE_COMMAND} -E make_directory "${PROJECT_SOURCE_DIR}/include/vendor/entityx/tags"
                   COMMAND cp -f "${PROJECT_SOURCE_DIR}/vendor/entityx/entityx/*.h"
                                 "${PROJECT_SOURCE_DIR}/include/vendor/entityx/"
                   COMMAND cp -f "${PROJECT_SOURCE_DIR}/vendor/entityx/entityx/3rdparty/*.h"
                                 "${PROJECT_SOURCE_DIR}/include/vendor/entityx/3rdparty/"
                   COMMAND cp -f "${PROJECT_SOURCE_DIR}/vendor/entityx/entityx/deps/*.h"
                                 "${PROJECT_SOURCE_DIR}/include/vendor/entityx/deps/"
                   COMMAND cp -f "${PROJECT_SOURCE_DIR}/vendor/entityx/entityx/help/*.h"
                                 "${PROJECT_SOURCE_DIR}/include/vendor/entityx/help/"
                   COMMAND cp -f "${PROJECT_SOURCE_DIR}/vendor/entityx/entityx/tags/*.h"
                                 "${PROJECT_SOURCE_DIR}/include/vendor/entityx/tags/")

# LICENSES
ADD_CUSTOM_COMMAND(TARGET ccncore POST_BUILD

                   # entityx
                   COMMAND ${CMAKE_COMMAND} -E copy
                       "${PROJECT_SOURCE_DIR}/vendor/entityx/COPYING"
                       "${PROJECT_SOURCE_DIR}/include/vendor/entityx/"

                   # glad
                   COMMAND ${CMAKE_COMMAND} -E make_directory "${PROJECT_SOURCE_DIR}/include/vendor/glad"
                   COMMAND ${CMAKE_COMMAND} -E copy
                       "${PROJECT_SOURCE_DIR}/vendor/glad/LICENSE"
                       "${PROJECT_SOURCE_DIR}/include/vendor/glad/"

                   # glfw
                   COMMAND ${CMAKE_COMMAND} -E make_directory "${PROJECT_SOURCE_DIR}/include/vendor/glfw"
                   COMMAND ${CMAKE_COMMAND} -E copy
                       "${PROJECT_SOURCE_DIR}/vendor/glfw/LICENSE.md"
                       "${PROJECT_SOURCE_DIR}/include/vendor/glfw/"

                   # glm
                   COMMAND ${CMAKE_COMMAND} -E copy
                       "${PROJECT_SOURCE_DIR}/vendor/glm/copying.txt"
                       "${PROJECT_SOURCE_DIR}/include/vendor/glm/"

                   # spdlog
                   COMMAND ${CMAKE_COMMAND} -E copy
                       "${PROJECT_SOURCE_DIR}/vendor/spdlog/LICENSE"
                       "${PROJECT_SOURCE_DIR}/include/vendor/spdlog/"

                   # stb
                   COMMAND ${CMAKE_COMMAND} -E make_directory "${PROJECT_SOURCE_DIR}/include/vendor/stb"
                   COMMAND ${CMAKE_COMMAND} -E copy
                       "${PROJECT_SOURCE_DIR}/vendor/stb/stb/LICENSE"
                       "${PROJECT_SOURCE_DIR}/include/vendor/stb/")

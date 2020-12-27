# EDITOR LIBRARY

add_library(ccneditor editor.cpp)

target_include_directories(ccneditor PRIVATE "${PROJECT_SOURCE_DIR}/include"
                                        "${PROJECT_SOURCE_DIR}/vendor/spdlog/include"
                                        "${PROJECT_SOURCE_DIR}/vendor/glfw/include"
                                        "${PROJECT_SOURCE_DIR}/vendor/glad/include"
                                        "${PROJECT_SOURCE_DIR}/vendor/glm"
                                        "${PROJECT_SOURCE_DIR}/vendor/imgui")

# imgui
add_subdirectory(${PROJECT_SOURCE_DIR}/vendor/imgui vendor/imgui/imgui)

set_target_properties(ccneditor PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib")

ADD_CUSTOM_COMMAND(TARGET ccneditor POST_BUILD

                   # imgui (partitioned in severeal header files copies)
                   COMMAND ${CMAKE_COMMAND} -E make_directory "${PROJECT_SOURCE_DIR}/include/vendor/imgui"
                   COMMAND ${CMAKE_COMMAND} -E make_directory "${PROJECT_SOURCE_DIR}/include/vendor/imgui/backends"
                   COMMAND cp -f "${PROJECT_SOURCE_DIR}/vendor/imgui/imgui/*.h"
                                 "${PROJECT_SOURCE_DIR}/include/vendor/imgui/"

                   COMMAND cp -f "${PROJECT_SOURCE_DIR}/vendor/imgui/imgui/backends/*.h"
                                 "${PROJECT_SOURCE_DIR}/include/vendor/imgui/backends/")

# LICENSES
ADD_CUSTOM_COMMAND(TARGET ccneditor POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy
                       "${PROJECT_SOURCE_DIR}/vendor/imgui/imgui/LICENSE.txt"
                       "${PROJECT_SOURCE_DIR}/include/vendor/imgui")
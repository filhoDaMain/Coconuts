# EDITOR LIBRARY

add_library(ccneditor editor.cpp)

target_include_directories(ccneditor PRIVATE "${PROJECT_SOURCE_DIR}/include"
                                        "${PROJECT_SOURCE_DIR}/vendor/spdlog/include"
                                        "${PROJECT_SOURCE_DIR}/vendor/glfw/include"
                                        "${PROJECT_SOURCE_DIR}/vendor/glad/include")
# imgui
add_subdirectory(${PROJECT_SOURCE_DIR}/vendor/imgui vendor/imgui/imgui)

set_target_properties(ccneditor PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib")
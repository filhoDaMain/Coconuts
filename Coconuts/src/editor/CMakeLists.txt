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
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       "${PROJECT_SOURCE_DIR}/vendor/imgui/imgui"
                       "${PROJECT_SOURCE_DIR}/include/vendor/imgui"
                   COMMENT "Copying 3rd party header files indirectly included by Sandbox App into ./include/vendor/")

ADD_CUSTOM_COMMAND(TARGET ccneditor POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy
                       "${PROJECT_SOURCE_DIR}/vendor/imgui/imgui/LICENSE.txt"
                       "${PROJECT_SOURCE_DIR}/include/vendor/imgui"
                   COMMENT "Copying 3rd party LICENSE files into ./include/vendor/")
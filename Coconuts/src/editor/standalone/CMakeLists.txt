# Standalone Binary Source Code
add_executable(ccneditor_standalone main.cpp MainApp.cpp EditorLayer.cpp GameLayer.cpp)

target_include_directories(ccneditor_standalone PRIVATE "${PROJECT_SOURCE_DIR}/include"
                                        "${PROJECT_SOURCE_DIR}/vendor/spdlog/include"
                                        "${PROJECT_SOURCE_DIR}/vendor/glfw/include"
                                        "${PROJECT_SOURCE_DIR}/vendor/glad/include"
                                        "${PROJECT_SOURCE_DIR}/vendor/glm"
                                        "${PROJECT_SOURCE_DIR}/vendor/imgui"
                                        "${PROJECT_SOURCE_DIR}/vendor/entityx")

# Coconuts + Editor + vendor Lib Linkage
target_link_libraries(ccneditor_standalone  "${PROJECT_SOURCE_DIR}/lib/libccncore.a"
                                            "${PROJECT_SOURCE_DIR}/lib/libccneditor.a"
                                            "${PROJECT_SOURCE_DIR}/lib/libglfw3.a"
                                            "${PROJECT_SOURCE_DIR}/lib/libglad.a"
                                            "${PROJECT_SOURCE_DIR}/lib/libentityx.a")

# Output directory
set_target_properties(ccneditor_standalone PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")

# Platform libraries linkage
if(APPLE)
    target_link_libraries(ccneditor_standalone  "-framework Cocoa"
                                "-framework OpenGL"
                                "-framework IOKit")
# GNU:
elseif(UNIX AND NOT APPLE)
    find_package(OpenGL REQUIRED)
    target_link_libraries(ccneditor_standalone OpenGL::GL)
    target_link_libraries(ccneditor_standalone "-ldl" "-pthread")
endif()

# AFTER COMPILATION INFO
ADD_CUSTOM_COMMAND(TARGET ccneditor_standalone POST_BUILD
                   COMMENT "Third-party header files and their Licenses were exported into [include/vendor/]")
# Standalone Binary Source Code
add_executable(ccneditor StandaloneApp.cpp EditorLayer.cpp ed_utils.cpp)

target_include_directories(ccneditor PRIVATE "${PROJECT_SOURCE_DIR}/include"
                                        "${PROJECT_SOURCE_DIR}/vendor/spdlog/include"
                                        "${PROJECT_SOURCE_DIR}/vendor/glfw/include"
                                        "${PROJECT_SOURCE_DIR}/vendor/glad/include"
                                        "${PROJECT_SOURCE_DIR}/vendor/glm"
                                        "${PROJECT_SOURCE_DIR}/vendor/imgui"
                                        "${PROJECT_SOURCE_DIR}/vendor/entityx"
                                        "${PROJECT_SOURCE_DIR}/vendor/nativefiledialog")

add_subdirectory(menu_bar)
add_subdirectory(panels)
add_subdirectory(popups)
add_subdirectory(${PROJECT_SOURCE_DIR}/vendor/imgui vendor/imgui/imgui)

# Third-party library
# nativefiledialog
add_subdirectory(${PROJECT_SOURCE_DIR}/vendor/nativefiledialog vendor/nativefiledialog/nativefiledialog)
set_target_properties(nfd PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib")

# Build nfd before
add_dependencies(ccneditor nfd)

# Coconuts + Editor + vendor Lib Linkage
target_link_libraries(ccneditor  "${PROJECT_SOURCE_DIR}/lib/libccncore.a"
                                            "${PROJECT_SOURCE_DIR}/lib/libglfw3.a"
                                            "${PROJECT_SOURCE_DIR}/lib/libglad.a"
                                            "${PROJECT_SOURCE_DIR}/lib/libnfd.a"
                                            )

if (${CMAKE_BUILD_TYPE} MATCHES "DEBUG")
   target_link_libraries(ccneditor "${PROJECT_SOURCE_DIR}/lib/libentityx-d.a"
                                   "${PROJECT_SOURCE_DIR}/lib/libyaml-cppd.a"
   )
else()
   target_link_libraries(ccneditor "${PROJECT_SOURCE_DIR}/lib/libentityx.a"
                                   "${PROJECT_SOURCE_DIR}/lib/libyaml-cpp.a"
)
endif()

# Output directory
set_target_properties(ccneditor PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")

# Platform libraries linkage
# macOS / OSX:
if(APPLE)
    target_link_libraries(ccneditor  "-framework Cocoa"
                                                "-framework OpenGL"
                                                "-framework IOKit"
                                                "-framework AppKit")
# GNU:
elseif(UNIX AND NOT APPLE)
    find_package(OpenGL REQUIRED)
    target_link_libraries(ccneditor OpenGL::GL)
    target_link_libraries(ccneditor "-ldl" "-lpthread" "-lGL")
endif()

# AFTER COMPILATION INFO
ADD_CUSTOM_COMMAND(TARGET ccneditor POST_BUILD
                   COMMENT "Third-party header files and their Licenses were exported into [include/vendor/]")
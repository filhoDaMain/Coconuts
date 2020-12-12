# Example Source Code
add_executable(tiles2d main.cpp MainApp.cpp GameLayer.cpp EditorLayer.cpp)

# Coconuts Includes
target_include_directories(tiles2d PRIVATE "${PROJECT_SOURCE_DIR}/../Coconuts/include"
                                        "${PROJECT_SOURCE_DIR}/../Coconuts/include/vendor")
# Coconuts Linkage
target_link_libraries(tiles2d  "${PROJECT_SOURCE_DIR}/../Coconuts/lib/libccncore.a"
                            "${PROJECT_SOURCE_DIR}/../Coconuts/lib/libccneditor.a"
                            "${PROJECT_SOURCE_DIR}/../Coconuts/lib/libglfw3.a"
                            "${PROJECT_SOURCE_DIR}/../Coconuts/lib/libglad.a")
# Output directory
set_target_properties(tiles2d PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/examples/tiles2d/bin/")

if(APPLE)
    target_link_libraries(tiles2d  "-framework Cocoa"
                                "-framework OpenGL"
                                "-framework IOKit")
# GNU:
elseif(UNIX AND NOT APPLE)
    find_package(OpenGL REQUIRED)
    target_link_libraries(tiles2d OpenGL::GL)
    target_link_libraries(tiles2d "-ldl" "-pthread")
endif()


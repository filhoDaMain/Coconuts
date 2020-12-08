# Example Source Code
add_executable(demo demo.cpp CameraController.cpp)

#target_include_directories(demo PRIVATE "${PROJECT_SOURCE_DIR}/include" "${PROJECT_SOURCE_DIR}/../Coconuts/include" "${PROJECT_SOURCE_DIR}/../Coconuts/include/vendor")

# Coconuts Includes
target_include_directories(demo PRIVATE "${PROJECT_SOURCE_DIR}/../Coconuts/include"
                                        "${PROJECT_SOURCE_DIR}/../Coconuts/include/vendor")
# Coconuts Linkage
target_link_libraries(demo  "${PROJECT_SOURCE_DIR}/../Coconuts/lib/libccncore.a"
                            "${PROJECT_SOURCE_DIR}/../Coconuts/lib/libccneditor.a"
                            "${PROJECT_SOURCE_DIR}/../Coconuts/lib/libglfw3.a"
                            "${PROJECT_SOURCE_DIR}/../Coconuts/lib/libglad.a")
# Output directory
set_target_properties(demo PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/examples/batch2d/bin/")

if(APPLE)
    target_link_libraries(demo  "-framework Cocoa"
                                "-framework OpenGL"
                                "-framework IOKit")
# GNU:
elseif(UNIX AND NOT APPLE)
    find_package(OpenGL REQUIRED)
    target_link_libraries(demo OpenGL::GL)
    target_link_libraries(demo "-ldl" "-pthread")
endif()


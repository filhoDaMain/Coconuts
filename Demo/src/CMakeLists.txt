add_executable(demo demo.cpp)

target_include_directories(demo PRIVATE "${PROJECT_SOURCE_DIR}/include" "${PROJECT_SOURCE_DIR}/../Coconuts/include" "${PROJECT_SOURCE_DIR}/../Coconuts/include/vendor")
set_target_properties(demo PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")

target_link_libraries(demo  "${PROJECT_SOURCE_DIR}/../Coconuts/lib/libcore.a"
                            "${PROJECT_SOURCE_DIR}/../Coconuts/lib/libglfw3.a")

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


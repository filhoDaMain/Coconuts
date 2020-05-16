# CORE LIBRARY

add_library(ccncore core.cpp application/Application.cpp logger/Logger.cpp event_system/EventDispatcher.cpp)

# Platform Specific files
# Mac OS
if (APPLE)
    message(STATUS "MacOS Platform detected")
    add_subdirectory(platform/MacOS)
elseif(UNIX AND NOT APPLE)
    message(STATUS "GNU Platform detected")
    add_subdirectory(platform/GNU)
endif()

target_include_directories(ccncore PRIVATE "${PROJECT_SOURCE_DIR}/include"
                                        "${PROJECT_SOURCE_DIR}/vendor/spdlog/include"
                                        "${PROJECT_SOURCE_DIR}/vendor/glfw/include")

set_target_properties(ccncore PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib")


# GLFW
if(NOT TARGET glfw)
    # Stand-alone build
    find_package(glfw REQUIRED)
endif()

set_target_properties(glfw PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib")
target_link_libraries(ccncore glfw)


ADD_CUSTOM_COMMAND(TARGET ccncore POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       "${PROJECT_SOURCE_DIR}/vendor/spdlog/include"
                       "${PROJECT_SOURCE_DIR}/include/vendor"
                   COMMENT "Copying 3rd party header files indirectly included by Sandbox App into ./include/vendor/")

ADD_CUSTOM_COMMAND(TARGET ccncore POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy
                       "${PROJECT_SOURCE_DIR}/vendor/spdlog/LICENSE"
                       "${PROJECT_SOURCE_DIR}/include/vendor/spdlog"
                   COMMENT "Copying 3rd party LICENSE files into ./include/vendor/")
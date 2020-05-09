# CORE LIBRARY

add_library(core core.cpp Application.cpp logger/Logger.cpp)

target_include_directories(core PRIVATE "${PROJECT_SOURCE_DIR}/include" "${PROJECT_SOURCE_DIR}/vendor/spdlog/include")

set_target_properties(core PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib")

ADD_CUSTOM_COMMAND(TARGET core POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       "${PROJECT_SOURCE_DIR}/vendor/spdlog/include"
                       "${PROJECT_SOURCE_DIR}/include/vendor"
                   COMMENT "Copying 3rd party header files into ./include/vendor/")

ADD_CUSTOM_COMMAND(TARGET core POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy
                       "${PROJECT_SOURCE_DIR}/vendor/spdlog/LICENSE"
                       "${PROJECT_SOURCE_DIR}/include/vendor/spdlog"
                   COMMENT "Copying 3rd party LICENSE files into ./include/vendor/")
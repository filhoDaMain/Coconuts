# CORE LIBRARY

add_library(core core.cpp Application.cpp)

target_include_directories(core PRIVATE "${PROJECT_SOURCE_DIR}/include" "${PROJECT_SOURCE_DIR}/vendor/spdlog/include")

set_target_properties(core PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib")

target_link_libraries(core libspdlog.a)
add_library(core core.cpp Application.cpp)

target_include_directories(core PRIVATE "${PROJECT_SOURCE_DIR}/include")
set_target_properties(core PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib")
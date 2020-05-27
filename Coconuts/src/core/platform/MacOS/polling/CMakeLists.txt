# CORE LIBRARY

# Input Polling

target_sources(ccncore PRIVATE "${PROJECT_SOURCE_DIR}/src/core/platform/MacOS/polling/MacPolling.cpp")
target_include_directories(ccncore PRIVATE "${PROJECT_SOURCE_DIR}/src/core/platform/MacOS/polling/")

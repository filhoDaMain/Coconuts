# CORE LIBRARY

# Window System

target_sources(ccncore PRIVATE "${PROJECT_SOURCE_DIR}/src/core/platform/MacOS/window_system/MacWindow.cpp")
target_include_directories(ccncore PRIVATE "${PROJECT_SOURCE_DIR}/src/core/platform/MacOS/window_system/")

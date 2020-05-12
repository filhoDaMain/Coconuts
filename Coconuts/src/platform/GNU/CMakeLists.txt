# CORE LIBRARY

# Window System

target_sources(ccncore PRIVATE platform/MacOS/window_system/MacWindow.cpp)

target_include_directories(ccncore PRIVATE platform/MacOS/window_system/)

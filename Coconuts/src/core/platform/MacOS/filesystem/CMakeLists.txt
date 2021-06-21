# CORE LIBRARY - Platform / MacOS / File System

# Source files
target_sources(ccncore PRIVATE  "${CMAKE_CURRENT_SOURCE_DIR}/MacFileSystem.cpp")

# Local header files
target_include_directories(ccncore PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")
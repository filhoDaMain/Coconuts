# CORE LIBRARY - Platform / GNU / File System

# Source files
target_sources(ccncore PRIVATE  "${CMAKE_CURRENT_SOURCE_DIR}/GNUFileSystem.cpp")

# Local header files
target_include_directories(ccncore PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")
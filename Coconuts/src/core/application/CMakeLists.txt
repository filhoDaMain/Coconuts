# CORE LIBRARY - Application

# Source files
target_sources(ccncore PRIVATE  "${CMAKE_CURRENT_SOURCE_DIR}/Application.cpp"
                                "${CMAKE_CURRENT_SOURCE_DIR}/AppManager.cpp")

# Local header files
target_include_directories(ccncore PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")
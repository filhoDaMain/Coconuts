# CORE LIBRARY - Layer System

# Source files
target_sources(ccncore PRIVATE  "${CMAKE_CURRENT_SOURCE_DIR}/Layer.cpp"
                                "${CMAKE_CURRENT_SOURCE_DIR}/LayerStack.cpp")

# Local header files
target_include_directories(ccncore PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")
# CORE LIBRARY - Asset Manager

# Source files
target_sources(ccncore PRIVATE  "${CMAKE_CURRENT_SOURCE_DIR}/AssetManager.cpp"
                                "${CMAKE_CURRENT_SOURCE_DIR}/LoadingRefs.cpp")

# Local header files
target_include_directories(ccncore PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")
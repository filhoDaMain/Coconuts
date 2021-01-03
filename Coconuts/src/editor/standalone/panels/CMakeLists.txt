# Standalone Binary - Panels

# Source files
target_sources(ccneditor_standalone PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/Viewport.cpp"
                                            "${CMAKE_CURRENT_SOURCE_DIR}/Statistics.cpp"
                                            "${CMAKE_CURRENT_SOURCE_DIR}/SceneOverview.cpp")

# Local header files
target_include_directories(ccncore PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")
# Standalone Binary - Panels

# Source files
target_sources(ccneditor_standalone PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/FileMenu.cpp"
                                            "${CMAKE_CURRENT_SOURCE_DIR}/EntityMenu.cpp"
                                            "${CMAKE_CURRENT_SOURCE_DIR}/AssetsMenu.cpp")

# Local header files
target_include_directories(ccncore PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")
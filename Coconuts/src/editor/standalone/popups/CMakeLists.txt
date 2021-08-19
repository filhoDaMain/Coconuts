# Standalone Binary - Panels

# Source files
target_sources(ccneditor_standalone PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/ImportTexture2DPopUp.cpp"
                                            "${CMAKE_CURRENT_SOURCE_DIR}/CreateSpritePopUp.cpp"
                                            "${CMAKE_CURRENT_SOURCE_DIR}/LoadProjectPopUp.cpp"
                                            "${CMAKE_CURRENT_SOURCE_DIR}/SaveProjectPopUp.cpp")

# Local header files
target_include_directories(ccncore PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")
# CORE LIBRARY - Cameras

# Source files
target_sources(ccncore PRIVATE  "${CMAKE_CURRENT_SOURCE_DIR}/OrthographicCamera.cpp"
                                "${CMAKE_CURRENT_SOURCE_DIR}/CameraController.cpp")

# Local header files
target_include_directories(ccncore PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")
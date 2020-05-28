# CORE LIBRARY

# graphics - OpenGL

target_sources(ccncore PRIVATE "${PROJECT_SOURCE_DIR}/src/core/platform/OpenGL/OpenGLGraphicsContext.cpp")
target_include_directories(ccncore PRIVATE "${PROJECT_SOURCE_DIR}/src/core/platform/OpenGL")
# CORE LIBRARY

# graphics - OpenGL

target_sources(ccncore PRIVATE  "${PROJECT_SOURCE_DIR}/src/core/platform/OpenGL/OpenGLGraphicsContext.cpp"
                                "${PROJECT_SOURCE_DIR}/src/core/platform/OpenGL/OpenGLVertexBuffer.cpp"
                                "${PROJECT_SOURCE_DIR}/src/core/platform/OpenGL/OpenGLIndexBuffer.cpp"
                                "${PROJECT_SOURCE_DIR}/src/core/platform/OpenGL/OpenGLVertexArray.cpp"
                                "${PROJECT_SOURCE_DIR}/src/core/platform/OpenGL/OpenGLRendererAPI.cpp"
                                "${PROJECT_SOURCE_DIR}/src/core/platform/OpenGL/OpenGLShader.cpp"
                                "${PROJECT_SOURCE_DIR}/src/core/platform/OpenGL/OpenGLTexture.cpp")

target_include_directories(ccncore PRIVATE "${PROJECT_SOURCE_DIR}/src/core/platform/OpenGL")
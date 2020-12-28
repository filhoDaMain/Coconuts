# CORE LIBRARY - Graphics

# Source files
target_sources(ccncore PRIVATE  "${CMAKE_CURRENT_SOURCE_DIR}/Shader.cpp"
                                "${CMAKE_CURRENT_SOURCE_DIR}/VertexBuffer.cpp"
                                "${CMAKE_CURRENT_SOURCE_DIR}/IndexBuffer.cpp"
                                "${CMAKE_CURRENT_SOURCE_DIR}/VertexArray.cpp"
                                "${CMAKE_CURRENT_SOURCE_DIR}/Renderer.cpp"
                                "${CMAKE_CURRENT_SOURCE_DIR}/RendererAPI.cpp"
                                "${CMAKE_CURRENT_SOURCE_DIR}/LowLevelAPI.cpp"
                                "${CMAKE_CURRENT_SOURCE_DIR}/Texture.cpp"
                                "${CMAKE_CURRENT_SOURCE_DIR}/Renderer2D.cpp"
                                "${CMAKE_CURRENT_SOURCE_DIR}/Sprite.cpp"
                                "${CMAKE_CURRENT_SOURCE_DIR}/Framebuffer.cpp")

# Local header files
target_include_directories(ccncore PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")
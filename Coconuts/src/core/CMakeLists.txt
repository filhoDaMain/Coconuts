# CORE LIBRARY

add_library(ccncore core.cpp
                    application/Application.cpp
                    logger/Logger.cpp
                    event_system/EventDispatcher.cpp
                    layer_system/Layer.cpp
                    layer_system/LayerStack.cpp
                    cameras/OrthographicCamera.cpp
                    cameras/CameraController.cpp
                    graphics/Shader.cpp
                    graphics/Shader.cpp
                    graphics/VertexBuffer.cpp
                    graphics/IndexBuffer.cpp
                    graphics/VertexArray.cpp
                    graphics/Renderer.cpp
                    graphics/RendererAPI.cpp
                    graphics/LowLevelAPI.cpp
                    graphics/Texture.cpp
                    graphics/Renderer2D.cpp
                    graphics/Sprite.cpp
                    graphics/Framebuffer.cpp)

# Platform Specific files
# Mac OS
if (APPLE)
    message(STATUS "MacOS Platform detected")
    message(STATUS "Using OpenGL Graphics API")
    add_subdirectory(platform/MacOS)
    add_subdirectory(platform/OpenGL)
elseif(UNIX AND NOT APPLE)
    message(STATUS "GNU Platform detected")
    add_subdirectory(platform/GNU)
endif()

target_include_directories(ccncore PRIVATE "${PROJECT_SOURCE_DIR}/include"
                                        "${PROJECT_SOURCE_DIR}/vendor/spdlog/include"
                                        "${PROJECT_SOURCE_DIR}/vendor/glfw/include"
                                        "${PROJECT_SOURCE_DIR}/vendor/glad/include"
                                        "${PROJECT_SOURCE_DIR}/vendor/glm"
                                        "${PROJECT_SOURCE_DIR}/vendor/stb")

set_target_properties(ccncore PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib")


# Copy 3rd party header files meant to be available to client

# Delete last
ADD_CUSTOM_COMMAND(TARGET ccncore POST_BUILD COMMAND rm -rf "${PROJECT_SOURCE_DIR}/include/vendor")

# Copy
ADD_CUSTOM_COMMAND(TARGET ccncore POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       "${PROJECT_SOURCE_DIR}/vendor/spdlog/include"
                       "${PROJECT_SOURCE_DIR}/include/vendor"
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       "${PROJECT_SOURCE_DIR}/vendor/glm/glm"
                       "${PROJECT_SOURCE_DIR}/include/vendor/glm"
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       "${PROJECT_SOURCE_DIR}/vendor/entt/src"
                       "${PROJECT_SOURCE_DIR}/include/vendor"
                   COMMENT "Copying 3rd party header files")

ADD_CUSTOM_COMMAND(TARGET ccncore POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy
                       "${PROJECT_SOURCE_DIR}/vendor/spdlog/LICENSE"
                       "${PROJECT_SOURCE_DIR}/include/vendor/spdlog"
                   COMMAND ${CMAKE_COMMAND} -E copy
                       "${PROJECT_SOURCE_DIR}/vendor/glm/copying.txt"
                       "${PROJECT_SOURCE_DIR}/include/vendor/glm"
                   COMMENT "Including 3rd party LICENSE files")

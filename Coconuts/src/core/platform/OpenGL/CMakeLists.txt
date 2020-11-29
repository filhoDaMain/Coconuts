# CORE LIBRARY

# graphics - OpenGL

# Set path for build byproducts for preprocessed Default GLSL Shaders directory
set(BUILD_BYPRODUCTS_OPENGL_DIR "${BUILD_BYPRODUCTS_DIR}/OpenGL")

# Create build byproducts directory for preprocessed Default GLSL Shaders
file(MAKE_DIRECTORY "${BUILD_BYPRODUCTS_OPENGL_DIR}")

# Proprocess Default Shaders and output them to BUILD_BYPRODUCTS_OPENGL_DIR

# INPUT
set(IN_DEFAULT_FRAGMENT_GLSL_FILEPATH "${DEFAULT_SHADER_SRCDIR}/GLSL_Default.frag")
set(IN_DEFAULT_VERTEX_GLSL_FILEPATH "${DEFAULT_SHADER_SRCDIR}/GLSL_Default.vert")

# OUTPUT
set(OUT_DEFAULT_FRAGMENT_GLSL_FILEPATH "${BUILD_BYPRODUCTS_OPENGL_DIR}/GLSL_Default.frag")
set(OUT_DEFAULT_VERTEX_GLSL_FILEPATH "${BUILD_BYPRODUCTS_OPENGL_DIR}/GLSL_Default.vert")

# Generate shader files with shaders' source code as cpp string literals
add_custom_target(defaultshaders ALL
                  COMMAND rm -f ${OUT_DEFAULT_FRAGMENT_GLSL_FILEPATH} && echo "R\"(" | cat - ${IN_DEFAULT_FRAGMENT_GLSL_FILEPATH} >> ${OUT_DEFAULT_FRAGMENT_GLSL_FILEPATH} && echo ")\"" >> ${OUT_DEFAULT_FRAGMENT_GLSL_FILEPATH}
                  COMMAND rm -f ${OUT_DEFAULT_VERTEX_GLSL_FILEPATH} && echo "R\"(" | cat - ${IN_DEFAULT_VERTEX_GLSL_FILEPATH} >> ${OUT_DEFAULT_VERTEX_GLSL_FILEPATH} && echo ")\"" >> ${OUT_DEFAULT_VERTEX_GLSL_FILEPATH}
                  BYPRODUCTS ${OUT_DEFAULT_FRAGMENT_GLSL_FILEPATH} ${OUT_DEFAULT_VERTEX_GLSL_FILEPATH}
                  VERBATIM
                 )

# Run defaultshaders pre-processing always before main target
add_dependencies(ccncore defaultshaders)

# Source files for OpenGL platform
target_sources(ccncore PRIVATE  "${PROJECT_SOURCE_DIR}/src/core/platform/OpenGL/OpenGLGraphicsContext.cpp"
                                "${PROJECT_SOURCE_DIR}/src/core/platform/OpenGL/OpenGLVertexBuffer.cpp"
                                "${PROJECT_SOURCE_DIR}/src/core/platform/OpenGL/OpenGLIndexBuffer.cpp"
                                "${PROJECT_SOURCE_DIR}/src/core/platform/OpenGL/OpenGLVertexArray.cpp"
                                "${PROJECT_SOURCE_DIR}/src/core/platform/OpenGL/OpenGLRendererAPI.cpp"
                                "${PROJECT_SOURCE_DIR}/src/core/platform/OpenGL/OpenGLShader.cpp"
                                "${PROJECT_SOURCE_DIR}/src/core/platform/OpenGL/OpenGLTexture.cpp")

target_include_directories(ccncore PRIVATE "${PROJECT_SOURCE_DIR}/src/core/platform/OpenGL")

# Create macros to access generated files path by code
target_compile_definitions(ccncore PRIVATE COCONUTS_DEFAULT_GLSL_SHADER_FRAGMENT="${OUT_DEFAULT_FRAGMENT_GLSL_FILEPATH}" COCONUTS_DEFAULT_GLSL_SHADER_VERTEX="${OUT_DEFAULT_VERTEX_GLSL_FILEPATH}")
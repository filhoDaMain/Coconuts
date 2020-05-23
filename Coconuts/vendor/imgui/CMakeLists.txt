# NOTE:
#
#   This CMakeLists.txt does not belong to the original imgui project.
#
#   It was added in order to facilitate the imgui source code integration
#   into the Coconuts Editor library build tree.
#
#
#   The imgui directory within this directory is the pristine imgui repo.
#
#


# EDITOR Library 

#Include imgui .cpp files in ccneditor target
file(GLOB IMGUI_SRC_FILES ${PROJECT_SOURCE_DIR}/vendor/imgui/imgui/*.cpp
# Workaround! Include   examples/imgui_impl_opengl3.cpp example
                          ${PROJECT_SOURCE_DIR}/vendor/imgui/imgui/examples/imgui_impl_opengl3.cpp
# Workaround! Include   examples/imgui_impl_glfw.cpp example
                          ${PROJECT_SOURCE_DIR}/vendor/imgui/imgui/examples/imgui_impl_glfw.cpp)
target_sources(ccneditor PRIVATE ${IMGUI_SRC_FILES})

#Include imgui headers in ccneditor target
target_include_directories(ccneditor PRIVATE "${PROJECT_SOURCE_DIR}/vendor/imgui"
# Workaround to enable local includes from examples/imgui_impl_opengl3.cpp example
                                             "${PROJECT_SOURCE_DIR}/vendor/imgui/imgui")

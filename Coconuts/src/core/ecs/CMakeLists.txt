# CORE LIBRARY - ECS

# Source files
target_sources(ccncore PRIVATE  "${CMAKE_CURRENT_SOURCE_DIR}/Scene.cpp"
                                "${CMAKE_CURRENT_SOURCE_DIR}/Entity.cpp"
                                "${CMAKE_CURRENT_SOURCE_DIR}/Serializer.cpp"
                                "${CMAKE_CURRENT_SOURCE_DIR}/SceneManager.cpp")

# Local header files
target_include_directories(ccncore PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")

add_subdirectory(systems)
add_subdirectory(event_handlers)
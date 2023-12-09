add_library(mesh mesh_data.cpp)
target_link_libraries(mesh PRIVATE glm)

add_subdirectory(opengl)
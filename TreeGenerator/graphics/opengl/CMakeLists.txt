add_library(graphics_opengl opengl_renderer.cpp)
target_link_libraries(graphics_opengl PRIVATE glad glm)

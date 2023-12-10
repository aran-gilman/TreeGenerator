add_library(graphics_opengl
	opengl_renderer.cpp
	opengl_window.cpp
)

target_link_libraries(graphics_opengl
	PUBLIC
		glm

	PRIVATE
		glad
		glfw
)

add_library(graphics_opengl)

target_sources(graphics_opengl
	PUBLIC
		opengl_renderer.h
		opengl_window.h

	PRIVATE
		opengl_renderer.cpp
		opengl_window.cpp
)

target_link_libraries(graphics_opengl
	PUBLIC
		glm
		imgui

	PRIVATE
		glad
		glfw
)

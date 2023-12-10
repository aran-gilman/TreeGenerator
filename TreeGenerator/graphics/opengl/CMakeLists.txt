add_library(graphics_opengl)

target_sources(graphics_opengl
	PUBLIC
		opengl_renderer.h
		opengl_window.h
		shader_program.h
		typed_shader.h

	PRIVATE
		opengl_renderer.cpp
		opengl_window.cpp
		shader_program.cpp
)

target_link_libraries(graphics_opengl
	PUBLIC
		glm
		glad

	PRIVATE
		glfw
)

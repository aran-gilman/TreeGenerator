add_library(graphics_opengl)

target_sources(graphics_opengl
	PUBLIC
		opengl_render_context.h
		opengl_window.h

	PRIVATE
		opengl_render_context.cpp
		opengl_window.cpp

		internal/opengl_camera.h
		internal/opengl_camera.cpp
		internal/opengl_mesh_renderer.h
		internal/opengl_mesh_renderer.cpp
		internal/shader_program.h
		internal/shader_program.cpp
		internal/typed_shader.h
)

target_link_libraries(graphics_opengl
	PUBLIC
		glm
		imgui

	PRIVATE
		glad
		glfw
)

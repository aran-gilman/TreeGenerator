add_library(graphics_opengl)

target_sources(graphics_opengl
	PUBLIC
		opengl_render_context.h
		opengl_window.h

	PRIVATE
		opengl_render_context.cpp
		opengl_window.cpp

		internal/gl_helper.h
		internal/opengl_camera.h
		internal/opengl_camera.cpp
		internal/opengl_mesh_renderer.h
		internal/opengl_mesh_renderer.cpp
		internal/shader_program.h
		internal/shader_program.cpp
		internal/typed_shader.h
)

if (NOT EMSCRIPTEN)
	target_link_libraries(graphics_opengl
		PUBLIC
			glm
			imgui

		PRIVATE
			glad
			glfw
	)
else()
	target_link_libraries(graphics_opengl
		PUBLIC
			glm
			imgui

		PRIVATE
			glfw
	)	
	set_target_properties(graphics_opengl
		PROPERTIES
		LINK_FLAGS " -s USE_GLFW=3 -s ASSERTIONS=1")
endif()

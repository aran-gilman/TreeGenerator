add_subdirectory(graphics)
add_subdirectory(lsystem)
add_subdirectory(imgui)
add_subdirectory(input)

add_executable (TreeGenerator)

target_sources(TreeGenerator
	PRIVATE
		tree_generator_app.h

		main.cpp
		tree_generator_app.cpp
)

target_link_libraries(TreeGenerator
	PRIVATE
		glm

		graphics_common
		graphics_opengl
		imgui
		imgui_demo
		input		
		lsystem_core
		lsystem_mesh_generator
		lsystem_string_generator
)

if (EMSCRIPTEN)
	target_link_libraries(TreeGenerator
		PUBLIC
			glfw
	)
	set_target_properties(TreeGenerator
		PROPERTIES
		SUFFIX ".html"
		LINK_FLAGS " -s USE_GLFW=3 -s WASM=1 -s ALLOW_MEMORY_GROWTH=1 -s NO_EXIT_RUNTIME=0 -s ASSERTIONS=1 -s MIN_WEBGL_VERSION=2")
endif()
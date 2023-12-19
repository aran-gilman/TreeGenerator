add_subdirectory(utility)
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
		imgui_extensions
		input		
		lsystem_core
		lsystem_mesh_generator
)
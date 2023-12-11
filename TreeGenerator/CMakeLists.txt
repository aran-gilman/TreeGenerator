add_subdirectory(graphics)
add_subdirectory(lsystem)
add_subdirectory(input)

add_executable (TreeGenerator)

target_sources(TreeGenerator
	PRIVATE
		main.cpp
)

target_link_libraries(TreeGenerator
	PRIVATE
		glm

		graphics_common
		graphics_opengl
		input		
		lsystem_core
		lsystem_mesh_generator
		lsystem_string_generator
)
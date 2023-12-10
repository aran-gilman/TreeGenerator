add_subdirectory(graphics)
add_subdirectory(lsystem)

add_executable (TreeGenerator)

target_sources(TreeGenerator
	PRIVATE
		main.cpp
)

target_link_libraries(TreeGenerator
	PRIVATE
		lsystem
		graphics_common
		graphics_opengl
)
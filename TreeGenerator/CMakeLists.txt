# CMakeList.txt : CMake project for TreeGenerator, include source and define
# project specific logic here.
#

add_subdirectory(graphics)
add_subdirectory(lsystem)

# Add source to this project's executable.
add_executable (TreeGenerator "main.cpp")

target_link_libraries(TreeGenerator
	PRIVATE
		lsystem
		graphics_common
		graphics_opengl
)
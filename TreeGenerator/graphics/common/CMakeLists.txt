add_library(graphics_common
	mesh_data.h
	mesh_data.cpp
	window.cpp
)

target_link_libraries(graphics_common
	PUBLIC
		glm
		
	PRIVATE
		glad
		glfw
)
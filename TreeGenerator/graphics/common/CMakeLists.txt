add_library(graphics_common
	mesh_data.h
	mesh_data.cpp
)

target_link_libraries(graphics_common
	PUBLIC
		glm
)
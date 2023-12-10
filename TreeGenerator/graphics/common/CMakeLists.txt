add_library(graphics_common)

target_sources(graphics_common
	PUBLIC
		camera_data.h
		mesh_data.h
		renderer.h
		window.h

	PRIVATE
		mesh_data.cpp
)

target_link_libraries(graphics_common
	PUBLIC
		glm
)
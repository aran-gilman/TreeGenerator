add_library(graphics_common)

target_sources(graphics_common
	PUBLIC
		camera_data.h
		key_action.h
		key_token.h
		mesh_data.h
		render_context.h
		transform.h
		window.h

	PRIVATE
		mesh_data.cpp
)

target_link_libraries(graphics_common
	PUBLIC
		glm
)
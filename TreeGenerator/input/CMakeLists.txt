add_library(input)

target_sources(input
	PUBLIC
		camera_controller.h

	PRIVATE
		camera_controller.cpp
)

target_link_libraries(input 
	PUBLIC
		graphics_common

	PRIVATE
		glm
)
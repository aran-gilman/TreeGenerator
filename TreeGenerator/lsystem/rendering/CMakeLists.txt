add_library(lsystem_rendering)

target_sources(lsystem_rendering
	PUBLIC
		text_renderer.h

	PRIVATE
		text_renderer.cpp
)

target_link_libraries(lsystem_rendering
	PUBLIC
		lsystem_core
)
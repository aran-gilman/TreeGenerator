add_library(lsystem_mesh_generator)
target_sources(lsystem_mesh_generator
	PUBLIC
		mesh_definition.h
		mesh_generator.h
		mesh_generator_action.h

	PRIVATE
		mesh_definition.cpp
		mesh_generator.cpp
		mesh_generator_action.cpp
)
target_link_libraries(lsystem_mesh_generator
	PUBLIC
		glm
		imgui

		graphics_common
		lsystem_core
		tree_generator_utility

	PRIVATE
		imgui_extensions
)

enable_testing()
include(GoogleTest)

add_executable(lsystem_mesh_generator_test)
target_sources(lsystem_mesh_generator_test
	PRIVATE
		mesh_generator.h
		mesh_generator_test.cpp
)
target_link_libraries(lsystem_mesh_generator_test
	PRIVATE
		GTest::gtest
		GTest::gmock
		GTest::gtest_main

		glm
		
		lsystem_core
		lsystem_mesh_generator
)
gtest_discover_tests(lsystem_mesh_generator_test)

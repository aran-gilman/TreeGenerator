add_library(lsystem_string_generator)
target_sources(lsystem_string_generator
	PUBLIC
		string_generator.h
	PRIVATE
		string_generator.cpp
)
target_link_libraries(lsystem_string_generator
	PUBLIC
		lsystem_core
)

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
)

enable_testing()
include(GoogleTest)

add_executable(lsystem_string_generator_test)
target_sources(lsystem_string_generator_test
	PRIVATE
		string_generator.h
		string_generator_test.cpp
)
target_link_libraries(lsystem_string_generator_test
	PRIVATE
		GTest::gtest
		GTest::gmock
		GTest::gtest_main
		
		lsystem_core
		lsystem_string_generator
)
gtest_discover_tests(lsystem_string_generator_test)

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

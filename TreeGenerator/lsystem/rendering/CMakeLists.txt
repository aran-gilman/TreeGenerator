add_library(lsystem_rendering)

target_sources(lsystem_rendering
	PUBLIC
		string_generator.h

	PRIVATE
		string_generator.cpp
)

target_link_libraries(lsystem_rendering
	PUBLIC
		lsystem_core
)

enable_testing()
add_executable(lsystem_rendering_test)

target_sources(lsystem_rendering_test
	PRIVATE
		string_generator_test.cpp
)

target_link_libraries(lsystem_rendering_test
	PRIVATE
		GTest::gtest
		GTest::gmock
		GTest::gtest_main
		
		lsystem_core
		lsystem_rendering
)

include(GoogleTest)
gtest_discover_tests(lsystem_rendering_test)
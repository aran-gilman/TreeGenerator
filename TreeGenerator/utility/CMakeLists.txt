add_library(tree_generator_utility INTERFACE)

target_sources(tree_generator_utility
	INTERFACE
		enum_helper.h
		error_handling.h
)

enable_testing()
include(GoogleTest)

add_executable(tree_generator_utility_enum_helper_test)
target_sources(tree_generator_utility_enum_helper_test
	PRIVATE
		enum_helper.h
		enum_helper_test.cpp
)
target_link_libraries(tree_generator_utility_enum_helper_test
	PRIVATE
		GTest::gtest
		GTest::gmock
		GTest::gtest_main
		
		tree_generator_utility
)
gtest_discover_tests(tree_generator_utility_enum_helper_test)
add_library(lsystem_core)

target_sources(lsystem_core
	PUBLIC
		lsystem.h

	PRIVATE
		lsystem.cpp
)

enable_testing()

add_executable(TreeGeneratorTest lsystem_test.cpp)

target_link_libraries(TreeGeneratorTest GTest::gtest GTest::gmock GTest::gtest_main lsystem_core)

include(GoogleTest)
gtest_discover_tests(TreeGeneratorTest)
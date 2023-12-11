add_library(lsystem_core)

target_sources(lsystem_core
	PUBLIC
		lsystem.h

	PRIVATE
		lsystem.cpp
)

enable_testing()
add_executable(lsystem_test)

target_sources(lsystem_test
	PRIVATE
		lsystem_test.cpp
)

target_link_libraries(lsystem_test
	PRIVATE
		GTest::gtest
		GTest::gmock
		GTest::gtest_main
		
		lsystem_core
)

include(GoogleTest)
gtest_discover_tests(lsystem_test)
add_library(lsystem_core)

target_sources(lsystem_core
	PUBLIC
		lsystem.h
		lsystem_parser.h

	PRIVATE
		lsystem.cpp
		lsystem_parser.cpp
)

enable_testing()
include(GoogleTest)

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

add_executable(lsystem_parser_test)
target_sources(lsystem_parser_test
	PRIVATE
		lsystem_parser_test.cpp
)
target_link_libraries(lsystem_parser_test
	PRIVATE
		GTest::gtest
		GTest::gmock
		GTest::gtest_main
		
		lsystem_core
)
gtest_discover_tests(lsystem_parser_test)
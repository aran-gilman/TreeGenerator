add_library(json_utility)
target_sources(json_utility
	PUBLIC
		lsystem_json.h

	PRIVATE
		lsystem_json.cpp
)
target_link_libraries(json_utility
	PUBLIC
		lsystem_core
		nlohmann_json::nlohmann_json
)

add_executable(json_utility_test)
target_sources(json_utility_test
	PRIVATE
		lsystem_json_test.cpp
)
target_link_libraries(json_utility_test
	PRIVATE
		GTest::gtest
		GTest::gmock
		GTest::gtest_main

		json_utility
		lsystem_core
)
gtest_discover_tests(json_utility_test)
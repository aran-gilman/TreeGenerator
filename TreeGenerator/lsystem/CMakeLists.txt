add_library(lsystem lsystem.cpp)

enable_testing()

add_executable(TreeGeneratorTest lsystem_test.cpp)

target_link_libraries(TreeGeneratorTest GTest::gtest GTest::gmock GTest::gtest_main lsystem)

include(GoogleTest)
gtest_discover_tests(TreeGeneratorTest)
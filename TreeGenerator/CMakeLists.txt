# CMakeList.txt : CMake project for TreeGenerator, include source and define
# project specific logic here.
#

add_library(lsystem lsystem.cpp)
set_property(TARGET lsystem PROPERTY CXX_STANDARD 20)

# Add source to this project's executable.
add_executable (TreeGenerator "main.cpp"  "window.cpp")
set_property(TARGET TreeGenerator PROPERTY CXX_STANDARD 20)

target_link_libraries(TreeGenerator PRIVATE glad glfw glm lsystem)

# Set up tests
enable_testing()

add_executable(TreeGeneratorTest lsystem_test.cpp)
set_property(TARGET TreeGeneratorTest PROPERTY CXX_STANDARD 20)

target_link_libraries(TreeGeneratorTest GTest::gtest GTest::gmock GTest::gtest_main lsystem)

include(GoogleTest)
gtest_discover_tests(TreeGeneratorTest)
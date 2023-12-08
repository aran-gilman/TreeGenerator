# CMakeList.txt : CMake project for TreeGenerator, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (TreeGenerator "main.cpp"  "Window.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET TreeGenerator PROPERTY CXX_STANDARD 20)
endif()

target_link_libraries(TreeGenerator PRIVATE glad glfw)

# Set up tests
enable_testing()

add_executable(example_test example_test.cpp)
target_link_libraries(example_test GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(example_test)
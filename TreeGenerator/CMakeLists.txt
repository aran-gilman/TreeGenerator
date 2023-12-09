# CMakeList.txt : CMake project for TreeGenerator, include source and define
# project specific logic here.
#

add_subdirectory(graphics)

add_library(lsystem lsystem.cpp)

# Add source to this project's executable.
add_executable (TreeGenerator "main.cpp")

target_link_libraries(TreeGenerator
	PRIVATE
		lsystem
		graphics_common
		graphics_opengl
)

# Set up tests
enable_testing()

add_executable(TreeGeneratorTest lsystem_test.cpp)

target_link_libraries(TreeGeneratorTest GTest::gtest GTest::gmock GTest::gtest_main lsystem)

include(GoogleTest)
gtest_discover_tests(TreeGeneratorTest)
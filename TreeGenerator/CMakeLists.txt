# CMakeList.txt : CMake project for TreeGenerator, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (TreeGenerator "main.cpp" )

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET TreeGenerator PROPERTY CXX_STANDARD 20)
endif()

target_link_libraries(TreeGenerator PRIVATE glad glfw)

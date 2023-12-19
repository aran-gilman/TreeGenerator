cmake_minimum_required (VERSION 3.14)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

# Configure global values and initialize the project.
set(CMAKE_CXX_STANDARD 20)
project (TreeGenerator VERSION 0.1.0)

# Create a config file to provide CMake-defined variables to the C++ code.
configure_file(config.h.in config.h)
message("Project binary dir: ${PROJECT_BINARY_DIR}")
include_directories("${PROJECT_BINARY_DIR}")

# Download and set up dependencies.
include(FetchContent)

FetchContent_Declare(
	glad
	GIT_REPOSITORY https://github.com/Dav1dde/glad.git
)
FetchContent_Declare(
	glfw
	GIT_REPOSITORY https://github.com/glfw/glfw.git
)
FetchContent_Declare(
	glm
	GIT_REPOSITORY https://github.com/g-truc/glm.git
	EXCLUDE_FROM_ALL
)
FetchContent_Declare(
	googletest
	GIT_REPOSITORY https://github.com/google/googletest.git
	GIT_TAG main
)
FetchContent_Declare(
	imgui
	GIT_REPOSITORY https://github.com/ocornut/imgui.git
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(glad glfw glm googletest imgui)

# Set up testing.
enable_testing()
include(GoogleTest)

# Include sub-projects.
add_subdirectory ("TreeGenerator")

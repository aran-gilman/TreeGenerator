#include <cstdlib>
#include <iostream>
#include <memory>

#include <glad/glad.h>

#include "Window.h"

using ::tree_generator::Window;

int main()
{
	try
	{
		std::unique_ptr<Window> window = Window::Create(800, 600, "TreeGenerator");
		window->Display([&]() {
			glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
			});
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

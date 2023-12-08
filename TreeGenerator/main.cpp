#include <cstdlib>
#include <iostream>
#include <memory>

#include "Window.h"

using ::tree_generator::Window;

int main()
{
	try
	{
		std::unique_ptr<Window> window = Window::Create(800, 600, "TreeGenerator");
		window->Display();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

#include <cstdlib>
#include <iostream>

#include "tree_generator_app.h"

int main()
{
	try
	{
		::tree_generator::TreeGeneratorApp app;
		app.Run();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

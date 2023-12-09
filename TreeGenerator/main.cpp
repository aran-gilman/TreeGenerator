#include <cstdlib>
#include <iostream>
#include <memory>

#include "graphics/common/mesh_data.h"
#include "graphics/common/renderer.h"
#include "graphics/common/window.h"
#include "graphics/opengl/opengl_renderer.h"

namespace tree_generator
{
	void Run()
	{
		std::unique_ptr<Window> window = Window::Create(800, 600, "TreeGenerator");
		std::unique_ptr<Renderer> renderer = std::make_unique<opengl::OpenGLRenderer>();

		renderer->AddMesh(::tree_generator::CreateQuad());

		window->Display([&]() {
			renderer->Render();
			});
	}
}

int main()
{
	try
	{
		::tree_generator::Run();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

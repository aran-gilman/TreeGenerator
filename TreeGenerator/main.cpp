#include <cstdlib>
#include <iostream>
#include <memory>
#include <vector>

#include "graphics/mesh_data.h"
#include "graphics/renderer.h"
#include "graphics/opengl/opengl_renderer.h"
#include "window.h"

using ::tree_generator::MeshData;
using ::tree_generator::opengl::OpenGLRenderer;
using ::tree_generator::Renderer;
using ::tree_generator::Window;

int main()
{
	try
	{
		std::unique_ptr<Window> window = Window::Create(800, 600, "TreeGenerator");
		std::unique_ptr<Renderer> renderer = std::make_unique<OpenGLRenderer>();

		renderer->AddMesh(::tree_generator::CreateQuad());

		window->Display([&]() {
			renderer->Render();
			});
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

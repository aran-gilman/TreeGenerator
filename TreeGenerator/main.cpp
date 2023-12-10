#include <cstdlib>
#include <iostream>
#include <memory>

#include <glm/glm.hpp>

#include "graphics/common/mesh_data.h"
#include "graphics/common/renderer.h"
#include "graphics/common/window.h"
#include "graphics/opengl/opengl_renderer.h"
#include "graphics/opengl/opengl_window.h"

namespace tree_generator
{
	void Run()
	{
		std::unique_ptr<Window> window =
			std::make_unique<opengl::OpenGLWindow>(800, 600, "TreeGenerator");
		std::unique_ptr<Renderer> renderer =
			std::make_unique<opengl::OpenGLRenderer>(window.get());

		renderer->SetCameraPosition(glm::vec3(1.0f, 1.0f, -5.0f));
		renderer->AddMesh(::tree_generator::CreateCylinder(3));

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

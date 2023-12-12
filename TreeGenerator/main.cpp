#include <cstdlib>
#include <iostream>
#include <memory>

#include <glm/glm.hpp>
#include <imgui.h>

#include "graphics/common/mesh_data.h"
#include "graphics/common/renderer.h"
#include "graphics/common/window.h"
#include "graphics/opengl/opengl_renderer.h"
#include "graphics/opengl/opengl_window.h"
#include "input/camera_controller.h"
#include "lsystem/core/lsystem.h"
#include "lsystem/rendering/mesh_generator.h"
#include "lsystem/rendering/mesh_generator_action.h"
#include "lsystem/rendering/string_generator.h"

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

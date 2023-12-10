#include <cstdlib>
#include <iostream>
#include <memory>

#include <glm/glm.hpp>

#include "graphics/common/mesh_data.h"
#include "graphics/common/renderer.h"
#include "graphics/common/window.h"
#include "graphics/opengl/opengl_renderer.h"
#include "graphics/opengl/opengl_window.h"
#include "input/camera_controller.h"

namespace tree_generator
{
	void HandleCameraInput(CameraController* camera, KeyToken keyToken, KeyAction action)
	{
		CameraController::Movement& movement = camera->GetCurrentMovement();
		CameraController::MovementSettings& settings = camera->GetMovementSettings();

		if (action == KeyAction::Press)
		{
			if (keyToken == KeyToken::A)
			{
				movement.horizontalRotationVelocity -=
					settings.maxHorizontalRotationPerSecond;
			}
			else if (keyToken == KeyToken::D)
			{
				movement.horizontalRotationVelocity +=
					settings.maxHorizontalRotationPerSecond;
			}
			else if (keyToken == KeyToken::S)
			{
				movement.verticalRotationVelocity -=
					settings.maxVerticalRotationPerSecond;
			}
			else if (keyToken == KeyToken::W)
			{
				movement.verticalRotationVelocity +=
					settings.maxVerticalRotationPerSecond;
			}
		}
		else if (action == KeyAction::Release)
		{
			if (keyToken == KeyToken::A)
			{
				movement.horizontalRotationVelocity +=
					settings.maxHorizontalRotationPerSecond;
			}
			else if (keyToken == KeyToken::D)
			{
				movement.horizontalRotationVelocity -=
					settings.maxHorizontalRotationPerSecond;
			}
			else if (keyToken == KeyToken::S)
			{
				movement.verticalRotationVelocity +=
					settings.maxVerticalRotationPerSecond;
			}
			else if (keyToken == KeyToken::W)
			{
				movement.verticalRotationVelocity -=
					settings.maxVerticalRotationPerSecond;
			}
		}
	}

	void Run()
	{
		std::unique_ptr<Window> window =
			std::make_unique<opengl::OpenGLWindow>(800, 600, "TreeGenerator");
		std::unique_ptr<Renderer> renderer =
			std::make_unique<opengl::OpenGLRenderer>(window.get());

		CameraController cameraController(renderer.get());

		std::vector<Transform> instances;
		instances.push_back({ glm::vec3(1), glm::vec3(0), 1.0f });
		instances.push_back({ glm::vec3(3), glm::vec3(0), 1.0f });

		renderer->AddMesh(CreateCylinder(3), instances);
		renderer->AddMesh(CreateQuad(),
			{ glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(90.0f, 0.0f, 0.0f), 10.0f });

		window->SetKeyboardCallback([&](KeyToken keyToken, KeyAction action) {
			HandleCameraInput(&cameraController, keyToken, action);
			});

		window->Display([&](double elapsedTime) {
			cameraController.Update(elapsedTime);
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

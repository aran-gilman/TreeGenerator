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
#include "lsystem/core/lsystem.h"
#include "lsystem/rendering/mesh_generator.h"
#include "lsystem/rendering/mesh_generator_action.h"
#include "lsystem/rendering/string_generator.h"

namespace tree_generator
{
	namespace
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
				else if (keyToken == KeyToken::Comma)
				{
					movement.heightVelocity -= settings.maxHeightChangePerSecond;
				}
				else if (keyToken == KeyToken::Period)
				{
					movement.heightVelocity += settings.maxHeightChangePerSecond;
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
				else if (keyToken == KeyToken::Comma)
				{
					movement.heightVelocity += settings.maxHeightChangePerSecond;
				}
				else if (keyToken == KeyToken::Period)
				{
					movement.heightVelocity -= settings.maxHeightChangePerSecond;
				}
			}
		}

		void HandleScrollInput(CameraController* camera, double xOffset, double yOffset)
		{
			camera->GetCurrentMovement().remainingDistanceChange -= yOffset;
		}

		void Run()
		{
			std::unique_ptr<Window> window =
				std::make_unique<opengl::OpenGLWindow>(800, 600, "TreeGenerator");
			std::unique_ptr<Renderer> renderer =
				std::make_unique<opengl::OpenGLRenderer>(window.get());

			CameraController cameraController(renderer.get());

			lsystem::Symbol trunk{ '1' };
			lsystem::Symbol leaf{ '0' };

			lsystem::Symbol push{ '[' };
			lsystem::Symbol pop{ ']' };

			lsystem::Symbol rotateRight{ 'R' };
			lsystem::Symbol rotateLeft{ 'L' };
			lsystem::Symbol advance{ 'A' };

			lsystem::MeshGenerator generator;
			generator.Define(trunk, std::make_unique<lsystem::DrawAction>(CreateCylinder(8)));
			generator.Define(leaf, std::make_unique<lsystem::DrawAction>(CreateQuad()));

			generator.Define(push, std::make_unique<lsystem::SaveAction>());
			generator.Define(pop, std::make_unique<lsystem::RestoreAction>());

			glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 45.0f);
			generator.Define(rotateRight, std::make_unique<lsystem::RotateAction>(-rotation));
			generator.Define(rotateLeft, std::make_unique<lsystem::RotateAction>(rotation));
			generator.Define(advance, std::make_unique<lsystem::MoveAction>());

			lsystem::StringGenerator stringGenerator;
			stringGenerator.Define(trunk, "1");
			stringGenerator.Define(leaf, "0");
			stringGenerator.Define(push, "[");
			stringGenerator.Define(pop, "]");
			stringGenerator.Define(rotateRight, "R");
			stringGenerator.Define(rotateLeft, "L");
			stringGenerator.Define(advance, "A");

			lsystem::RuleMap rules = {
				{ trunk, { trunk, advance, trunk }},
				{ leaf, {
					trunk,
					push, rotateRight, advance, leaf, pop,
					rotateLeft, advance, leaf
			}} };
			std::vector<lsystem::Symbol> symbols = { leaf };
			for (int i = 0; i < 5; i++)
			{
				symbols = Iterate(symbols, rules);
			}

			std::cout << "Generated tree: " << stringGenerator.Generate(symbols) << std::endl;

			std::vector<lsystem::MeshGroup> meshes = generator.Generate(symbols);
			for (const lsystem::MeshGroup& group : meshes)
			{
				renderer->AddMesh(group.mesh, group.instances);
			}

			renderer->AddMesh(CreateQuad(),
				{ glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(90.0f, 0.0f, 0.0f), 10.0f });

			window->SetKeyboardCallback([&](KeyToken keyToken, KeyAction action) {
				HandleCameraInput(&cameraController, keyToken, action);
				});

			window->SetScrollCallback([&](double xOffset, double yOffset) {
				HandleScrollInput(&cameraController, xOffset, yOffset);
				});

			window->Display([&](double elapsedTime) {
				cameraController.Update(elapsedTime);
				renderer->Render();
				});
		}
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

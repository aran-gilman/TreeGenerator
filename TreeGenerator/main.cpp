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

		struct DisplaySymbols
		{
			lsystem::Symbol trunk{ '1' };
			lsystem::Symbol leaf{ '0' };

			lsystem::Symbol push{ '[' };
			lsystem::Symbol pop{ ']' };

			lsystem::Symbol rotateRight{ 'R' };
			lsystem::Symbol rotateLeft{ 'L' };
			lsystem::Symbol advance{ 'A' };
		};

		std::vector<lsystem::Symbol> CreateSimpleBinaryTree(
			const DisplaySymbols& symbols,
			int iterations)
		{
			lsystem::RuleMap rules = {
				{ symbols.trunk, { symbols.trunk, symbols.advance, symbols.trunk }},
				{ symbols.leaf, {
					symbols.trunk,
					symbols.push,
					symbols.rotateRight, symbols.advance, symbols.leaf,
					symbols.pop,
					symbols.rotateLeft, symbols.advance, symbols.leaf
			}} };

			std::vector<lsystem::Symbol> output = { symbols.leaf };
			for (int i = 0; i < iterations; i++)
			{
				output = Iterate(output, rules);
			}
			return output;
		}

		// See http://algorithmicbotany.org/papers/lsfp.pdf page 25
		// X = leaf
		// F = trunk
		// - = rotateRight
		// + = rotateLeft
		// The system in the book does not have an explicit advance, but we add it
		// to keep a 1:1 relationship between symbols and actions.
		std::vector<lsystem::Symbol> CreateTreeTypeB(
			const DisplaySymbols& symbols,
			int iterations)
		{
			lsystem::RuleMap rules = {
				{ symbols.trunk, { symbols.trunk, symbols.advance, symbols.trunk }},
				{ symbols.leaf, {
					symbols.trunk, symbols.rotateRight,
					symbols.push, symbols.push,
					symbols.advance, symbols.leaf,
					symbols.pop,
					symbols.rotateLeft, symbols.advance, symbols.leaf,
					symbols.pop,
					symbols.rotateLeft, symbols.advance, symbols.trunk,
					symbols.push,
					symbols.rotateLeft, symbols.advance, symbols.trunk,
					symbols.advance, symbols.leaf,
					symbols.pop,
					symbols.rotateRight, symbols.advance, symbols.leaf
			}} };

			std::vector<lsystem::Symbol> output = { symbols.leaf };
			for (int i = 0; i < iterations; i++)
			{
				output = Iterate(output, rules);
			}
			return output;
		}

		lsystem::MeshGenerator CreateBinaryTreeMeshGenerator(
			const DisplaySymbols& symbols,
			glm::vec3 rotation)
		{
			lsystem::MeshGenerator generator;
			generator.Define(
				symbols.trunk,
				std::make_unique<lsystem::DrawAction>(CreateCylinder(8, 0.5f, 0.5f)));
			generator.Define(
				symbols.leaf,
				std::make_unique<lsystem::DrawAction>(CreateQuad()));
			generator.Define(
				symbols.push,
				std::make_unique<lsystem::SaveAction>());
			generator.Define(
				symbols.pop,
				std::make_unique<lsystem::RestoreAction>());
			generator.Define(
				symbols.rotateRight,
				std::make_unique<lsystem::RotateAction>(-rotation));
			generator.Define(
				symbols.rotateLeft,
				std::make_unique<lsystem::RotateAction>(rotation));
			generator.Define(
				symbols.advance,
				std::make_unique<lsystem::MoveAction>(0.5f));
			return generator;
		}

		lsystem::StringGenerator CreateBinaryTreeStringGenerator(
			const DisplaySymbols& symbols)
		{
			lsystem::StringGenerator generator;
			generator.Define(symbols.trunk, "1");
			generator.Define(symbols.leaf, "0");
			generator.Define(symbols.push, "[");
			generator.Define(symbols.pop, "]");
			generator.Define(symbols.rotateRight, "R");
			generator.Define(symbols.rotateLeft, "L");
			generator.Define(symbols.advance, "A");
			return generator;
		}

		void Run()
		{
			std::unique_ptr<Window> window =
				std::make_unique<opengl::OpenGLWindow>(800, 600, "TreeGenerator");
			std::unique_ptr<Renderer> renderer =
				std::make_unique<opengl::OpenGLRenderer>(window.get());

			CameraController cameraController(renderer.get());

			DisplaySymbols symbols;
			lsystem::StringGenerator stringGenerator =
				CreateBinaryTreeStringGenerator(symbols);
			lsystem::MeshGenerator meshGenerator =
				CreateBinaryTreeMeshGenerator(symbols, glm::vec3(0.0f, 0.0f, 22.5f));

			window->SetKeyboardCallback([&](KeyToken keyToken, KeyAction action) {
				HandleCameraInput(&cameraController, keyToken, action);
				});

			window->SetScrollCallback([&](double xOffset, double yOffset) {
				HandleScrollInput(&cameraController, xOffset, yOffset);
				});

			bool showDemoWindow = false;
			int iterations = 5;
			bool doOutputToConsole = false;
			window->Display([&](double elapsedTime) {
				ImGui::Begin("Tree Generator");
				ImGui::InputInt("Iterations", &iterations);
				if (iterations < 1)
				{
					iterations = 1;
				}
				if (ImGui::Button("Regenerate"))
				{
					renderer->ClearAllMeshes();
					std::vector<lsystem::Symbol> tree = CreateTreeTypeB(symbols, iterations);
					if (doOutputToConsole)
					{
						std::cout << "Generated tree: " <<
							stringGenerator.Generate(tree) << std::endl;
					}
					std::vector<lsystem::MeshGroup> meshes = meshGenerator.Generate(tree);
					for (const lsystem::MeshGroup& group : meshes)
					{
						renderer->AddMesh(group.mesh, group.instances);
					}
				}
				ImGui::Checkbox("Output to console", &doOutputToConsole);
				if (ImGui::Button("Open Demo Window"))
				{
					showDemoWindow = true;
				}
				ImGui::End();

				if (showDemoWindow)
				{
					ImGui::ShowDemoWindow(&showDemoWindow);
				}
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

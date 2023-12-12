#include "tree_generator_app.h"

#include <iostream>

#include <glm/glm.hpp>
#include <imgui.h>

#include "graphics/common/mesh_data.h"
#include "graphics/common/renderer.h"
#include "graphics/common/window.h"
#include "graphics/opengl/opengl_renderer.h"
#include "graphics/opengl/opengl_window.h"
#include "input/camera_controller.h"
#include "lsystem/core/lsystem.h"
#include "lsystem/rendering/mesh_generator_action.h"

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

	TreeGeneratorApp::TreeGeneratorApp() :
		window_(std::make_unique<opengl::OpenGLWindow>(800, 600, "TreeGenerator")),
		renderer_(std::make_unique<opengl::OpenGLRenderer>(window_.get())),
		cameraController_(std::make_unique<CameraController>(renderer_.get())),
		symbols_({}),
		stringGenerator_(CreateBinaryTreeStringGenerator(symbols_)),
		meshGenerator_(
			CreateBinaryTreeMeshGenerator(symbols_, glm::vec3(0.0f, 0.0f, 22.5f)))
	{
		window_->SetKeyboardCallback([&](KeyToken keyToken, KeyAction action) {
			HandleCameraInput(cameraController_.get(), keyToken, action);
			});

		window_->SetScrollCallback([&](double xOffset, double yOffset) {
			HandleScrollInput(cameraController_.get(), xOffset, yOffset);
			});
	}

	void TreeGeneratorApp::Run()
	{
		bool showDemoWindow = false;
		int iterations = 5;
		bool doOutputToConsole = false;
		window_->Display([&](double elapsedTime) {
			ImGui::Begin("Tree Generator");
			ImGui::InputInt("Iterations", &iterations);
			if (iterations < 1)
			{
				iterations = 1;
			}
			if (ImGui::Button("Regenerate"))
			{
				renderer_->ClearAllMeshes();
				std::vector<lsystem::Symbol> tree = CreateTreeTypeB(symbols_, iterations);
				if (doOutputToConsole)
				{
					std::cout << "Generated tree: " <<
						stringGenerator_.Generate(tree) << std::endl;
				}
				std::vector<lsystem::MeshGroup> meshes = meshGenerator_.Generate(tree);
				for (const lsystem::MeshGroup& group : meshes)
				{
					renderer_->AddMesh(group.mesh, group.instances);
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
			cameraController_->Update(elapsedTime);
			renderer_->Render();
			});
	}
}
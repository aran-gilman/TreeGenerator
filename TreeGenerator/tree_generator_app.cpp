#include "tree_generator_app.h"

#include <algorithm>
#include <iostream>
#include <optional>

#include <glm/glm.hpp>
#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>

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
		// The system in the book does not have an explicit advance, but we add it
		// to keep a 1:1 relationship between symbols and actions.
		lsystem::StringLSystem CreateTreeTypeB()
		{
			lsystem::StringLSystem lSystem;
			lSystem.axiom = "X";
			lSystem.rules.push_back({ "F", "FAF" });
			lSystem.rules.push_back({ "X", "F-[[AX]+AX]+AF[+AFAX]-AX" });
			return lSystem;
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
	}

	TreeGeneratorApp::TreeGeneratorApp() :
		window_(std::make_unique<opengl::OpenGLWindow>(800, 600, "TreeGenerator")),
		renderer_(std::make_unique<opengl::OpenGLRenderer>(window_.get())),
		cameraController_(std::make_unique<CameraController>(renderer_.get())),

		symbols_({}),
		stringLSystem_(CreateTreeTypeB()),
		lSystem_(ParseLSystem(stringLSystem_)),

		stringGenerator_(CreateBinaryTreeStringGenerator(symbols_)),
		meshGenerator_(
			CreateBinaryTreeMeshGenerator(symbols_, glm::vec3(0.0f, 0.0f, 22.5f))),

		showDemoWindow_(false),
		iterations_(5),
		doOutputToConsole_(false)
	{
		window_->SetKeyboardCallback([&](KeyToken keyToken, KeyAction action) {
			HandleCameraInput(cameraController_.get(), keyToken, action);
			});

		window_->SetScrollCallback([&](double xOffset, double yOffset) {
			HandleScrollInput(cameraController_.get(), xOffset, yOffset);
			});
	}

	// The destructor must be define in the implementation file for forward-
	// declaring classes with unique_ptr to work.
	// See https://stackoverflow.com/questions/6012157/is-stdunique-ptrt-required-to-know-the-full-definition-of-t
	TreeGeneratorApp::~TreeGeneratorApp() = default;

	void TreeGeneratorApp::Run()
	{
		window_->Display([&](double elapsedTime) {
			ShowMenu();

			if (showDemoWindow_)
			{
				ImGui::ShowDemoWindow(&showDemoWindow_);
			}

			cameraController_->Update(elapsedTime);
			renderer_->Render();
			});
	}

	void TreeGeneratorApp::ShowMenu()
	{
		ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoTitleBar;
		ImGui::Begin("Tree Generator", nullptr, windowFlags);

		ShowGenerateButton();
		ShowLSystemSection();
		ShowDebugSection();

		ImGui::End();
	}

	void TreeGeneratorApp::ShowGenerateButton()
	{
		if (ImGui::Button("Generate"))
		{
			renderer_->ClearAllMeshes();
			lSystem_ = ParseLSystem(stringLSystem_);
			std::vector<lsystem::Symbol> tree = lsystem::Generate(lSystem_, iterations_);
			if (doOutputToConsole_)
			{
				std::cout << "Generated tree: " <<
					stringGenerator_.Generate(tree) << std::endl;
			}
			std::vector<lsystem::MeshGroup> meshes =
				meshGenerator_.Generate(tree);
			for (const lsystem::MeshGroup& group : meshes)
			{
				renderer_->AddMesh(group.mesh, group.instances);
			}
		}
	}

	void TreeGeneratorApp::ShowLSystemSection()
	{
		if (ImGui::CollapsingHeader("L-System"))
		{
			if (ImGui::InputInt("Iterations", &iterations_) && iterations_ < 1)
			{
				iterations_ = 1;
			}

			const float buttonSize = ImGui::GetFrameHeight();
			int toRemove = -1;
			for (int i = 0; i < stringLSystem_.rules.size(); ++i)
			{
				ImGui::PushID(i);
				auto& [successor, predecessor] = stringLSystem_.rules[i];
				ImGui::InputText("Predecessor", &successor);
				ImGui::SameLine();
				if (ImGui::Button("-", ImVec2(buttonSize, buttonSize)))
				{
					toRemove = i;
				}
				ImGui::InputText("Successor", &predecessor);
				ImGui::PopID();
			}

			if (toRemove >= 0)
			{
				auto iter = stringLSystem_.rules.begin() + toRemove;
				stringLSystem_.rules.erase(iter, iter + 1);
			}

			if (ImGui::Button("Add rule"))
			{
				stringLSystem_.rules.push_back({});
			}
		}
	}

	void TreeGeneratorApp::ShowDebugSection()
	{
		if (ImGui::CollapsingHeader("Debug"))
		{
			ImGui::Checkbox("Output to console", &doOutputToConsole_);
			if (ImGui::Button("Open Demo Window"))
			{
				showDemoWindow_ = true;
			}
		}
	}
}
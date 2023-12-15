#include "tree_generator_app.h"

#include <algorithm>
#include <format>
#include <iostream>
#include <optional>
#include <string_view>

#include <glm/glm.hpp>
#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>

#include <config.h>

#include "graphics/common/camera.h"
#include "graphics/common/mesh_data.h"
#include "graphics/common/mesh_renderer.h"
#include "graphics/common/render_context.h"
#include "graphics/common/window.h"
#include "graphics/opengl/opengl_render_context.h"
#include "graphics/opengl/opengl_window.h"
#include "input/camera_controller.h"
#include "lsystem/core/lsystem.h"
#include "lsystem/rendering/mesh_definition.h"
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
			camera->GetCurrentMovement().remainingDistanceChange -= static_cast<float>(yOffset);
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

		lsystem::MeshGenerator CreateDefaultMeshGenerator(
			glm::vec3 rotation)
		{
			lsystem::Symbol trunk{ 'F' };
			lsystem::Symbol leaf{ 'X' };

			lsystem::Symbol push{ '[' };
			lsystem::Symbol pop{ ']' };

			lsystem::Symbol rotateRight{ '-' };
			lsystem::Symbol rotateLeft{ '+' };
			lsystem::Symbol advance{ 'A' };

			Material trunkMaterial = { {0.5f, 0.2f, 0.0f, 1.0f} };
			Material leafMaterial = { {0.0f, 0.5f, 0.0f, 1.0f} };

			lsystem::MeshGenerator generator;
			generator.Define(
				trunk,
				std::make_unique<lsystem::DrawAction>(
					std::make_unique<lsystem::CylinderDefinition>(8, 0.15f, 0.1f),
					trunkMaterial));
			generator.Define(
				leaf,
				std::make_unique<lsystem::DrawAction>(
					std::make_unique<lsystem::QuadDefinition>(),
					leafMaterial));
			generator.Define(
				push, std::make_unique<lsystem::SaveAction>());
			generator.Define(
				pop, std::make_unique<lsystem::RestoreAction>());
			generator.Define(
				rotateRight, std::make_unique<lsystem::RotateAction>(-rotation));
			generator.Define(
				rotateLeft,	std::make_unique<lsystem::RotateAction>(rotation));
			generator.Define(
				advance, std::make_unique<lsystem::MoveAction>(0.15f));

			return generator;
		}
	}

	TreeGeneratorApp::TreeGeneratorApp() :
		window_(std::make_unique<opengl::OpenGLWindow>(
			800, 600, std::format("Tree Generator v{}", PROJECT_VERSION))),
		renderer_(std::make_unique<opengl::OpenGLRenderContext>()),

		camera_(renderer_->CreateCamera()),
		cameraController_(std::make_unique<CameraController>(camera_.get())),

		stringLSystem_(CreateTreeTypeB()),
		lSystem_(ParseLSystem(stringLSystem_)),

		meshGenerator_(
			CreateDefaultMeshGenerator(glm::vec3(0.0f, 0.0f, 22.5f))),

		showDemoWindow_(false),
		iterations_(5),
		doOutputToConsole_(false),
		doShowNormals_(false)
	{
		window_->SetKeyboardCallback([&](KeyToken keyToken, KeyAction action) {
			HandleCameraInput(cameraController_.get(), keyToken, action);
			});

		window_->SetScrollCallback([&](double xOffset, double yOffset) {
			HandleScrollInput(cameraController_.get(), xOffset, yOffset);
			});

		camera_->SetViewport({ 0, 0, window_->Width(), window_->Height() });
		window_->SetFramebufferSizeCallback([&](int width, int height) {
			camera_->SetViewport({0, 0, width, height});
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
			camera_->Bind();

			MeshRenderer::RenderMode renderMode =
				doShowNormals_ ?
				MeshRenderer::RenderMode::Normals : 
				MeshRenderer::RenderMode::Material;
			for (int i = 0; i < meshes_.size(); ++i)
			{
				meshes_[i]->Render(renderMode);
			}
			});
	}

	void TreeGeneratorApp::ShowMenu()
	{
		ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoTitleBar;
		ImGui::Begin("Tree Generator", nullptr, windowFlags);

		ShowGenerateButton();
		ShowLSystemSection();
		ShowMeshSection();
		ShowDebugSection();

		ImGui::End();
	}

	void TreeGeneratorApp::ShowGenerateButton()
	{
		if (ImGui::Button("Generate"))
		{
			meshes_.clear();
			lSystem_ = ParseLSystem(stringLSystem_);
			std::vector<lsystem::Symbol> tree = lsystem::Generate(lSystem_, iterations_);
			if (doOutputToConsole_)
			{
				std::cout << "Generated tree: " <<
					ToString(tree) << std::endl;
			}
			std::vector<lsystem::MeshGroup> meshGroups =
				meshGenerator_.Generate(tree);
			for (const lsystem::MeshGroup& group : meshGroups)
			{
				auto mesh = renderer_->CreateMeshRenderer();
				mesh->SetMeshData(group.mesh, group.instances);
				mesh->SetMaterial(group.material);
				meshes_.push_back(std::move(mesh));
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

	void TreeGeneratorApp::ShowMeshSection()
	{
		if (ImGui::CollapsingHeader("Mesh"))
		{
			std::optional<lsystem::Symbol> toRemove = std::nullopt;
			for (auto& [symbol, action] : meshGenerator_.GetActionMap())
			{
				const static std::string unsetActionName = "No action set";
				std::string_view actionName = 
					action == nullptr ? unsetActionName : action->Name();
				std::string label = std::format(
					"{0}: {1}", lsystem::ToString(symbol), actionName);
				if (ImGui::TreeNode(label.c_str()))
				{
					if (action != nullptr)
					{
						action->ShowGUI();
					}

					if (ImGui::Button("Remove"))
					{
						toRemove = symbol;
					}
					ImGui::TreePop();
				}
			}

			if (toRemove.has_value())
			{
				meshGenerator_.Remove(toRemove.value());
			}

			if (ImGui::TreeNode("Define new symbol action"))
			{
				ImGui::InputText("Symbol", &newSymbolInput_);
				if (newSymbolInput_.size() > 1)
				{
					newSymbolInput_ = newSymbolInput_[0];
				}

				const bool disableAddButton =
					newSymbolInput_.size() == 0 || 
					meshGenerator_.HasDefinition(lsystem::ToSymbol(newSymbolInput_[0]));

				if (disableAddButton)
				{
					ImGui::BeginDisabled();
				}

				if (ImGui::Button("Add"))
				{
					meshGenerator_.Define(lsystem::ToSymbol(newSymbolInput_[0]), nullptr);
				}

				if (disableAddButton)
				{
					ImGui::EndDisabled();
				}
				ImGui::TreePop();
			}
		}
	}

	void TreeGeneratorApp::ShowDebugSection()
	{
		if (ImGui::CollapsingHeader("Debug"))
		{
			ImGui::Checkbox("Output to console", &doOutputToConsole_);
			ImGui::Checkbox("Show normals", &doShowNormals_);
			if (ImGui::Button("Open Demo Window"))
			{
				showDemoWindow_ = true;
			}
		}
	}
}
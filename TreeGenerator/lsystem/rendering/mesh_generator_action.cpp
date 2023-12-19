#include "mesh_generator_action.h"

#include <iostream>
#include <stdexcept>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <imgui.h>

#include "mesh_definition.h"

namespace tree_generator::lsystem
{
	namespace
	{
		Transform CreateTransform(const MeshGeneratorState& state)
		{
			return Transform { 
				state.positionStack.back(), 
				state.rotationStack.back(), 
				1.0f };
		}
	}

	// TODO: Consolidate the names with those in the child MeshGeneratorAction
	// classes.
	//
	// One complication is that these need to be static for all actions of a
	// given type, but the names of the MeshGeneratorAction objects can vary
	// from object to object of the same class. (e.g. Draw's name changes based
	// on the type of mesh being drawn).
	std::string GetName(MeshGeneratorActionType actionType)
	{
		switch (actionType)
		{
		case MeshGeneratorActionType::None:
			return "None";
		case MeshGeneratorActionType::Draw:
			return "Draw mesh";
		case MeshGeneratorActionType::Move:
			return "Move forward";
		case MeshGeneratorActionType::Rotate:
			return "Rotate";
		case MeshGeneratorActionType::Save:
			return "Push state";
		case MeshGeneratorActionType::Restore:
			return "Pop state";
		}
		return "Unknown action";
	}

	void MeshGeneratorAction::ShowGUI()
	{
		ImGui::Text("<No options>");
	}

	DrawAction::DrawAction(
		std::unique_ptr<MeshDefinition> meshDefinition, Material material) :
		meshDefinition_(std::move(meshDefinition)),
		material_(std::move(material))
	{
		if (meshDefinition_ == nullptr)
		{
			throw std::invalid_argument(
				"Failed to initialize DrawAction: meshDefinition must be non-null");
		}
		meshData_ = meshDefinition_->GenerateMesh();
	}

	void DrawAction::PerformAction(const Symbol& symbol, MeshGeneratorState* state)
	{
		if (auto iter = state->symbolMeshMap.find(symbol);
			iter != state->symbolMeshMap.end())
		{
			iter->second.instances.push_back(CreateTransform(*state));
		}
		else
		{
			state->symbolMeshMap.emplace(symbol,
				MeshGroup{ meshData_, { CreateTransform(*state) }, material_ });
		}
	}

	void DrawAction::ShowGUI()
	{
		MeshType currentMeshType = meshDefinition_->GetMeshType();
		if (ImGui::BeginCombo("Mesh Type", GetName(currentMeshType).c_str()))
		{
			MeshTypeIterator meshTypes{};
			for (auto iter = meshTypes.begin(); iter != meshTypes.end(); ++iter)
			{
				MeshType type = *iter;
				const bool isSelected = (type == currentMeshType);
				if (ImGui::Selectable(GetName(type).c_str(), isSelected))
				{
					std::unique_ptr<MeshDefinition> newDefinition =
						MeshDefinition::FromMeshType(type);
					if (newDefinition == nullptr)
					{
						std::cerr << "Invalid mesh type" << std::endl;
					}
					else
					{
						meshDefinition_ = std::move(newDefinition);
						meshData_ = meshDefinition_->GenerateMesh();
					}
				}
			}
			ImGui::EndCombo();
		}

		// These strings should be known at compile time, so eventually we will
		// want to construct this only once instead of every frame.
		std::vector<std::string> meshNames;

		if (meshDefinition_->ShowGUI())
		{
			meshData_ = meshDefinition_->GenerateMesh();
		}
		ImGui::ColorEdit4("Material color", glm::value_ptr(material_.color));
	}

	const std::string_view DrawAction::Name() const
	{
		return meshDefinition_->Name(); 
	}

	MoveAction::MoveAction(float distance) : distance_(distance) {}

	void MoveAction::PerformAction(const Symbol& symbol, MeshGeneratorState* state)
	{
		glm::vec3 rotation = state->rotationStack.back();

		glm::mat4 matrix = glm::mat4(1.0f);
		matrix = glm::rotate(matrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
		matrix = glm::rotate(matrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		matrix = glm::rotate(matrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));

		glm::vec3 direction = glm::vec3(0.0f, 1.0f, 0.0f);
		direction = glm::mat3(matrix) * direction;

		state->positionStack.back() += (direction * distance_);
	}

	void MoveAction::ShowGUI()
	{
		ImGui::InputFloat("Distance", &distance_, 0.01f, 0.1f);
	}

	RotateAction::RotateAction(glm::vec3 rotation) : rotation_(rotation) {}

	void RotateAction::PerformAction(const Symbol& symbol, MeshGeneratorState* state)
	{
		state->rotationStack.back() += rotation_;
	}

	void RotateAction::ShowGUI()
	{
		ImGui::InputFloat3("Angles (degrees)", &rotation_.x);
	}

	void SaveAction::PerformAction(const Symbol& symbol, MeshGeneratorState* state)
	{
		state->positionStack.push_back(state->positionStack.back());
		state->rotationStack.push_back(state->rotationStack.back());
	}

	void RestoreAction::PerformAction(const Symbol& symbol, MeshGeneratorState* state)
	{
		state->positionStack.pop_back();
		state->rotationStack.pop_back();
	}
}

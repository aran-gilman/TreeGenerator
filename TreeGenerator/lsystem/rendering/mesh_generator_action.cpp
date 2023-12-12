#include "mesh_generator_action.h"

#include <glm/gtc/matrix_transform.hpp>
#include <imgui.h>

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

	void MeshGeneratorAction::ShowGUI()
	{
		ImGui::Text("<No options>");
	}

	DrawAction::DrawAction(MeshData meshData)
		: meshData_(meshData)
	{
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
				MeshGroup{ meshData_, { CreateTransform(*state) } });
		}
	}

	void DrawAction::ShowGUI()
	{
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
	}

	RotateAction::RotateAction(glm::vec3 rotation) : rotation_(rotation) {}

	void RotateAction::PerformAction(const Symbol& symbol, MeshGeneratorState* state)
	{
		state->rotationStack.back() += rotation_;
	}

	void RotateAction::ShowGUI()
	{
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

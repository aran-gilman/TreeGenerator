#include "mesh_generator_action.h"

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

	DrawAction::DrawAction(MeshData meshData)
		: meshData_(meshData)
	{
	}

	void DrawAction::PerformAction(const Symbol& symbol, MeshGeneratorState* state)
	{
		if (auto iter = state->symbolMeshMap.find(symbol);
			iter != state->symbolMeshMap.end())
		{
			iter->second->instances.push_back(CreateTransform(*state));
		}
		else
		{
			state->meshes.push_back({ meshData_, { CreateTransform(*state) } });
			state->symbolMeshMap[symbol] = &state->meshes.back();
		}
	}

	void MoveAction::PerformAction(const Symbol& symbol, MeshGeneratorState* state)
	{

	}

	RotateAction::RotateAction(glm::vec3 rotation) : rotation_(rotation) {}

	void RotateAction::PerformAction(const Symbol& symbol, MeshGeneratorState* state)
	{

	}

	void SaveAction::PerformAction(const Symbol& symbol, MeshGeneratorState* state)
	{

	}

	void RestoreAction::PerformAction(const Symbol& symbol, MeshGeneratorState* state)
	{

	}
}

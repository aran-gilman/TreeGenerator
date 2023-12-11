#include "mesh_generator_action.h"

namespace tree_generator::lsystem
{
	DrawAction::DrawAction(MeshData meshData) : meshData_(meshData)
	{
	}

	void DrawAction::PerformAction(MeshGeneratorState* state)
	{
	}

	void MoveAction::PerformAction(MeshGeneratorState* state)
	{

	}

	RotateAction::RotateAction(glm::vec3 rotation) : rotation_(rotation) {}

	void RotateAction::PerformAction(MeshGeneratorState* state)
	{

	}

	void SaveAction::PerformAction(MeshGeneratorState* state)
	{

	}

	void RestoreAction::PerformAction(MeshGeneratorState* state)
	{

	}
}

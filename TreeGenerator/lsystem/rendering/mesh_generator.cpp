#include "mesh_generator.h"

#include <algorithm>
#include <iterator>

namespace tree_generator::lsystem
{
	void MeshGenerator::DefineDraw(const Symbol& symbol, MeshData meshData)
	{
		actions_[symbol] = std::make_unique<DrawAction>(std::move(meshData));
	}

	void MeshGenerator::DefineMove(const Symbol& symbol)
	{
		actions_[symbol] = std::make_unique<MoveAction>();
	}

	void MeshGenerator::DefineRotate(const Symbol& symbol, glm::vec3 rotation)
	{
		actions_[symbol] = std::make_unique<RotateAction>(rotation);
	}

	void MeshGenerator::DefineSave(const Symbol& symbol)
	{
		actions_[symbol] = std::make_unique<SaveAction>();
	}

	void MeshGenerator::DefineRestore(const Symbol& symbol)
	{
		actions_[symbol] = std::make_unique<RestoreAction>();
	}

	std::vector<MeshGroup> MeshGenerator::Generate(
		const std::vector<Symbol>& symbols) const
	{
		MeshGeneratorState state;
		state.positionStack.push_back(glm::vec3(0.0f));
		state.rotationStack.push_back(glm::vec3(0.0f));

		for (const Symbol& symbol : symbols)
		{
			if (auto iter = actions_.find(symbol); iter != actions_.end())
			{
				iter->second->PerformAction(symbol, &state);
			}
		}
		return state.meshes;
	}
}

#include "mesh_generator.h"

#include <algorithm>
#include <iterator>

namespace tree_generator::lsystem
{
	void MeshGenerator::Define(
		const Symbol& symbol,
		std::unique_ptr<MeshGeneratorAction> action)
	{
		actions_[symbol] = std::move(action);
	}

	void MeshGenerator::Remove(Symbol symbol)
	{
		actions_.erase(symbol);
	}
	
	bool MeshGenerator::HasDefinition(Symbol symbol)
	{
		return actions_.find(symbol) != actions_.end();
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
				if (iter->second != nullptr)
				{
					iter->second->PerformAction(symbol, &state);
				}
			}
		}

		std::vector<MeshGroup> meshes;
		for (auto& [symbol, meshGroup] : state.symbolMeshMap)
		{
			meshes.push_back(std::move(meshGroup));
		}
		return meshes;
	}
}

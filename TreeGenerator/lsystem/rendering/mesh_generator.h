#ifndef TREE_GENERATOR_LSYSTEM_MESH_GENERATOR_H_
#define TREE_GENERATOR_LSYSTEM_MESH_GENERATOR_H_

#include <memory>
#include <type_traits>
#include <unordered_map>
#include <vector>

#include <glm/glm.hpp>

#include "../core/lsystem.h"
#include "../../graphics/common/mesh_data.h"
#include "../../graphics/common/transform.h"
#include "mesh_generator_action.h"

namespace tree_generator::lsystem
{
	class MeshGenerator
	{
	public:
		// Define an action for the given symbol.
		void Define(const Symbol& symbol, std::unique_ptr<MeshGeneratorAction> action);

		std::vector<MeshGroup> Generate(const std::vector<Symbol>& symbols) const;

	private:
		std::unordered_map<Symbol, std::unique_ptr<MeshGeneratorAction>> actions_;
	};
}

#endif  // !TREE_GENERATOR_LSYSTEM_MESH_GENERATOR_H_
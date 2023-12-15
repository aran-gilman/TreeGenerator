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
		using ActionMap = std::unordered_map<Symbol, std::unique_ptr<MeshGeneratorAction>>;

		// Define an action for the given symbol.
		// If there is already an action for this symbol, the previous action
		// is overwritten.
		void Define(const Symbol& symbol, std::unique_ptr<MeshGeneratorAction> action);

		// Remove defined action for this symbol, if it exists.
		// Otherwise does nothing.
		void Remove(Symbol symbol);

		// Returns whether an action is defined for this symbol.
		bool HasDefinition(Symbol symbol);

		std::vector<MeshGroup> Generate(const std::vector<Symbol>& symbols) const;
		ActionMap& GetActionMap() { return actions_; }

	private:
		std::unordered_map<Symbol, std::unique_ptr<MeshGeneratorAction>> actions_;
	};
}

#endif  // !TREE_GENERATOR_LSYSTEM_MESH_GENERATOR_H_
#ifndef TREE_GENERATOR_LSYSTEM_H_
#define TREE_GENERATOR_LSYSTEM_H_

#include <map>
#include <vector>

namespace tree_generator
{
	namespace lsystem
	{
		enum class Symbol : unsigned char {};
		using RuleMap = std::map<Symbol, std::vector<Symbol>>;
		std::vector<Symbol> Iterate(const std::vector<Symbol>& previous, const RuleMap& rules);
	}
}

#endif  // !TREE_GENERATOR_LSYSTEM_H_
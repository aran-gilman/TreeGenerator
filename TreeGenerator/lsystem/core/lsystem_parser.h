#ifndef TREE_GENERATOR_LSYSTEM_PARSER_H_
#define TREE_GENERATOR_LSYSTEM_PARSER_H_

#include <string>
#include <vector>
#include <utility>

#include "lsystem.h"

namespace tree_generator::lsystem
{
	// We use a vector of pairs instead of a map because the first element
	// does not act like a usual map key. This is particularly relevant when
	// replacing existing rules, since we want to be able to change the first
	// element without erasing the entry and making a new one.
	using StringRuleMap = std::vector<std::pair<std::string, std::string>>;
	struct StringLSystem
	{
		std::string axiom;
		StringRuleMap rules;
	};	

	std::vector<Symbol> ParseSymbols(const std::string& str);
	LSystem ParseLSystem(const StringLSystem& stringLSystem);
}

#endif  // !TREE_GENERATOR_LSYSTEM_PARSER_H_
#ifndef TREE_GENERATOR_LSYSTEM_PARSER_H_
#define TREE_GENERATOR_LSYSTEM_PARSER_H_

#include <map>
#include <string>
#include <vector>

#include "lsystem.h"

namespace tree_generator::lsystem
{
	// We may want to use a different data structure for this in the long run.
	// It more or less works right now, when each symbol = one char, but
	// parameterized strings will require more complex handling.
	// e.g. F(x) -> F(x+1) where x could be any number
	using StringRuleMap = std::map<std::string, std::string>;
	struct StringLSystem
	{
		std::string axiom;
		StringRuleMap rules;
	};

	std::vector<Symbol> ParseSymbols(const std::string& str);
}

#endif  // !TREE_GENERATOR_LSYSTEM_PARSER_H_
#ifndef TREE_GENERATOR_LSYSTEM_PARSER_H_
#define TREE_GENERATOR_LSYSTEM_PARSER_H_

#include <vector>
#include <string>

#include "lsystem.h"

namespace tree_generator::lsystem
{
	std::vector<Symbol> ParseSymbols(const std::string& str);
}

#endif  // !TREE_GENERATOR_LSYSTEM_PARSER_H_
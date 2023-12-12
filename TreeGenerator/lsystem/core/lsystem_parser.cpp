#include "lsystem_parser.h"

namespace tree_generator::lsystem
{
	std::vector<Symbol> ParseSymbols(const std::string& str)
	{
		std::vector<Symbol> symbols;
		symbols.reserve(str.size());
		for (char c : str)
		{
			symbols.push_back(static_cast<Symbol>(c));
		}
		return symbols;
	}
}
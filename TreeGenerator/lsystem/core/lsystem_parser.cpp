#include "lsystem_parser.h"

#include <stdexcept>

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

	LSystem ParseLSystem(const StringLSystem& stringLSystem)
	{
		LSystem parsed;
		parsed.axiom = ParseSymbols(stringLSystem.axiom);
		for (const auto& [key, value] : stringLSystem.rules)
		{
			std::vector<Symbol> keySymbols = ParseSymbols(key);
			if (keySymbols.size() != 1)
			{
				throw std::runtime_error(
					"Could not parse rule: rule key must contain exactly one symbol");
			}
			parsed.rules.emplace(keySymbols[0], ParseSymbols(value));
		}
		return parsed;
	}
}
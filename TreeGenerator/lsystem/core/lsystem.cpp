#include "lsystem.h"

namespace tree_generator::lsystem
{
	std::vector<Symbol> Generate(const LSystem& lSystem, int iterations)
	{
		std::vector<Symbol> output(lSystem.axiom);
		for (int i = 0; i < iterations; ++i)
		{
			output = Iterate(output, lSystem.rules);
		}
		return output;
	}

	std::vector<Symbol> Iterate(const std::vector<Symbol>& previous, const RuleMap& rules)
	{
		std::vector<Symbol> next;
		for (Symbol symbol : previous)
		{
			if (auto iter = rules.find(symbol); iter != rules.end())
			{
				next.insert(
					std::end(next),
					std::begin(iter->second),
					std::end(iter->second));
			}
			else
			{
				next.push_back(symbol);
			}
		}
		return next;
	}

	std::string ToString(Symbol symbol)
	{
		char c = static_cast<char>(symbol);
		return std::string(1, c);
	}
}
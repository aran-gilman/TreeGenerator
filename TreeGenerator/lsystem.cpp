#include "lsystem.h"

namespace tree_generator
{
	namespace lsystem
	{
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
	}
}
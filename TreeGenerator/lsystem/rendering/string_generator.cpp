#include "string_generator.h"

namespace tree_generator
{
	namespace lsystem
	{
		void StringGenerator::Define(const Symbol& symbol, const std::string& representation)
		{
			symbolMap_[symbol] = representation;
		}

		std::string StringGenerator::Generate(const std::vector<Symbol>& symbols) const
		{
			std::string output;
			for (const Symbol& symbol : symbols)
			{
				if (auto iter = symbolMap_.find(symbol); iter != symbolMap_.end())
				{
					output.append(iter->second);
				}
			}
			return output;
		}
	}
}
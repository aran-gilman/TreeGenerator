#ifndef TREE_GENERATOR_LSYSTEM_TEXT_RENDERER_H_
#define TREE_GENERATOR_LSYSTEM_TEXT_RENDERER_H_

#include <string>
#include <vector>

#include "../core/lsystem.h"

namespace tree_generator
{
	namespace lsystem
	{
		class StringGenerator
		{
		public:
			void Define(const Symbol& symbol, const std::string& representation);
			std::string Generate(const std::vector<Symbol>& symbols) const;
		};
	}
}

#endif  // !TREE_GENERATOR_LSYSTEM_TEXT_RENDERER_H_
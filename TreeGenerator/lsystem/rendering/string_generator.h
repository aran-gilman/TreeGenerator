#ifndef TREE_GENERATOR_LSYSTEM_TEXT_RENDERER_H_
#define TREE_GENERATOR_LSYSTEM_TEXT_RENDERER_H_

#include <string>
#include <unordered_map>
#include <vector>

#include "../core/lsystem.h"

namespace tree_generator
{
	namespace lsystem
	{
		// Generates a string representation of an lsystem-generated vector
		class StringGenerator
		{
		public:
			// Registers a symbol and its representation with the generator.
			void Define(const Symbol& symbol, const std::string& representation);

			// Creates a string from the provided symbols based on previously
			// registered definitions.
			std::string Generate(const std::vector<Symbol>& symbols) const;

		private:
			std::unordered_map<Symbol, std::string> symbolMap_;
		};
	}
}

#endif  // !TREE_GENERATOR_LSYSTEM_TEXT_RENDERER_H_
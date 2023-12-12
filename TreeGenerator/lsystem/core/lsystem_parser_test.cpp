#include "lsystem_parser.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using ::testing::ElementsAre;
using ::testing::Pair;

namespace tree_generator::lsystem
{
	namespace
	{
		TEST(LSystemParserTest, ParseBasicSymbols)
		{
			EXPECT_THAT(
				ParseSymbols("Ab1!"),
				ElementsAre(
					Symbol{ 'A' },
					Symbol{ 'b' },
					Symbol{ '1' },
					Symbol{ '!' }
				));
		}

		TEST(LSystemParserTest, ParseStringLSystem)
		{
			StringLSystem stringLSystem;
			stringLSystem.axiom = "A";
			stringLSystem.rules = {
				{ "A", "BA" },
				{ "B", "A" }
			};

			Symbol A{ 'A' };
			Symbol B{ 'B' };

			LSystem parsedLSystem = ParseLSystem(stringLSystem);
			EXPECT_THAT(parsedLSystem.axiom, ElementsAre(A));
			EXPECT_THAT(
				parsedLSystem.rules,
				ElementsAre(
					Pair(A, ElementsAre(B, A)),
					Pair(B, ElementsAre(A))));
		}
	}
}
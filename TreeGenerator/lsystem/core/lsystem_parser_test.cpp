#include "lsystem_parser.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using ::testing::ElementsAre;

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
	}
}
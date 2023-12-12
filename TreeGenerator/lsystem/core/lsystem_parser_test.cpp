#include "lsystem_parser.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace tree_generator::lsystem
{
	namespace
	{
		TEST(LSystemParserTest, ExampleTest)
		{
			EXPECT_EQ(6 * 7, 42);
		}
	}
}
#include "lsystem_json.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using ::testing::ExplainMatchResult;
using ::testing::Eq;
using ::testing::UnorderedElementsAre;
using ::testing::Pair;

namespace tree_generator::lsystem
{
	namespace
	{
		TEST(LSystemJsonTest, LSystemToJson)
		{
			Symbol a{ 'a' };
			Symbol b{ 'b' };

			LSystem lSystem;
			lSystem.axiom = { a };
			lSystem.rules = {
				{ a, { a , b }},
				{ b, { a }}
			};

			nlohmann::json j = lSystem;

			EXPECT_EQ(j.dump(), R"({"axiom":"a","rules":{"a":"ab","b":"a"}})");
		}
	}
}
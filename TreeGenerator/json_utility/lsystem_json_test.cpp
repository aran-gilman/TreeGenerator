#include "lsystem_json.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using ::testing::ElementsAre;
using ::testing::Pair;
using ::testing::UnorderedElementsAre;

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

		TEST(LsystemJsonTest, JsonToLSystem)
		{
			nlohmann::json j = R"({"axiom":"a","rules":{"a":"ab","b":"a"}})";
			LSystem lSystem = j.template get<LSystem>();

			Symbol a{ 'a' };
			Symbol b{ 'b' };
			EXPECT_THAT(lSystem.axiom, ElementsAre(a));
			EXPECT_THAT(lSystem.rules,
				UnorderedElementsAre(
					Pair(a, ElementsAre(a, b)),
					Pair(b, ElementsAre(a))
				)
			);
		}
	}
}
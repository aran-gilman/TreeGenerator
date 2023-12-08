#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "lsystem.h"

using ::testing::ElementsAre;
using ::testing::ElementsAreArray;

using ::tree_generator::lsystem::Iterate;
using ::tree_generator::lsystem::RuleMap;
using ::tree_generator::lsystem::Symbol;

TEST(LSystemTest, IterateEmptyRuleMapNoChange)
{
	Symbol A{ 'a' };
	RuleMap rules;
	std::vector<Symbol> axiom{ A };
	EXPECT_THAT(Iterate(axiom, rules), ElementsAreArray(axiom));
}

TEST(LSystemTest, IterateReplacesOneToOne)
{
	Symbol A{ 'a' };
	Symbol B{ 'b' };
	RuleMap rules{ { A, { B }} };
	std::vector<Symbol> axiom{ A };
	EXPECT_THAT(Iterate(axiom, rules), ElementsAre(B));
}

TEST(LSystemTest, IterateReplacesOneToMany)
{
	Symbol A{ 'a' };
	Symbol B{ 'b' };
	RuleMap rules{ { A, { B, B }} };
	std::vector<Symbol> axiom{ A };
	EXPECT_THAT(Iterate(axiom, rules), ElementsAre(B, B));
}

TEST(LSystemTest, IterateReplacesMultipleRules)
{
	Symbol A{ 'a' };
	Symbol B{ 'b' };
	RuleMap rules{
		{ A, { A, B }},
		{ B, { A }}
	};
	std::vector<Symbol> axiom{ A, B };
	EXPECT_THAT(Iterate(axiom, rules), ElementsAre(A, B, A));
}

TEST(LSystemTest, IterateDoesNotChangeConstants)
{
	Symbol A{ 'a' };
	Symbol B{ 'b' };
	Symbol C{ 'c' };
	RuleMap rules{ { A, { B, B }} };
	std::vector<Symbol> axiom{ C, A, C };
	EXPECT_THAT(Iterate(axiom, rules), ElementsAre(C, B, B, C));
}

TEST(LSystemTest, IteratePerformsOneIterationOnly)
{
	Symbol A{ 'a' };
	Symbol B{ 'b' };
	RuleMap rules{ { A, { A, B }} };
	std::vector<Symbol> axiom{ A };
	EXPECT_THAT(Iterate(axiom, rules), ElementsAre(A, B));
}

TEST(LSystemTest, IterateDoesNotMutateAxiom)
{
	Symbol A{ 'a' };
	Symbol B{ 'b' };
	RuleMap rules{ { A, { B }} };
	std::vector<Symbol> axiom{ A };
	Iterate(axiom, rules);
	EXPECT_THAT(axiom, ElementsAre(A));
}

TEST(LSystemTest, IterateMultipleCalls)
{
	Symbol A{ 'a' };
	Symbol B{ 'b' };
	RuleMap rules{ { A, { B, A }} };
	std::vector<Symbol> axiom{ A };

	std::vector<Symbol> step0 = Iterate(axiom, rules);
	std::vector<Symbol> step1 = Iterate(step0, rules);
	std::vector<Symbol> step2 = Iterate(step1, rules);

	EXPECT_THAT(step2, ElementsAre(B, B, B, A));
}
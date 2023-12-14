#include "enum_helper.h"

#include <type_traits>
#include <vector>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using ::testing::ElementsAre;

namespace tree_generator::utility
{
	enum class TestEnum
	{
		A = 0,
		B,
		C
	};

	void PrintTo(const TestEnum& val, std::ostream* os)
	{
		*os << static_cast<std::underlying_type_t<TestEnum>>(val);
	}

	namespace
	{
		TEST(TreeGeneratorUtilityEnumHelperTest, EnumIteratorIteratesOverEnum)
		{
			auto enumIterator = EnumIterator<
				TestEnum, TestEnum::A, TestEnum::C>();

			std::vector<TestEnum> values;
			for (auto iter = enumIterator.begin(); iter != enumIterator.end(); ++iter)
			{
				values.push_back(*iter);
			}

			EXPECT_THAT(values,
				ElementsAre(TestEnum::A, TestEnum::B));
		}

		TEST(TreeGeneratorUtilityEnumHelperTest, EnumIteratorIteratesFromZero)
		{
			auto enumIterator = EnumIteratorFromZero<TestEnum, TestEnum::C>();

			std::vector<TestEnum> values;
			for (auto iter = enumIterator.begin(); iter != enumIterator.end(); ++iter)
			{
				values.push_back(*iter);
			}

			EXPECT_THAT(values,
				ElementsAre(TestEnum::A, TestEnum::B));
		}

		TEST(TreeGeneratorUtilityEnumHelperTest, EnumIteratorIteratesInclusive)
		{
			auto enumIterator = EnumIteratorInclusive<
				TestEnum, TestEnum::A, TestEnum::C>();

			std::vector<TestEnum> values;
			for (auto iter = enumIterator.begin(); iter != enumIterator.end(); ++iter)
			{
				values.push_back(*iter);
			}

			EXPECT_THAT(values,
				ElementsAre(TestEnum::A, TestEnum::B, TestEnum::C));
		}

		TEST(TreeGeneratorUtilityEnumHelperTest, EnumIteratorIteratesFromZeroInclusive)
		{
			auto enumIterator = EnumIteratorFromZeroInclusive<
				TestEnum, TestEnum::C>();

			std::vector<TestEnum> values;
			for (auto iter = enumIterator.begin(); iter != enumIterator.end(); ++iter)
			{
				values.push_back(*iter);
			}

			EXPECT_THAT(values,
				ElementsAre(TestEnum::A, TestEnum::B, TestEnum::C));
		}
	}
}
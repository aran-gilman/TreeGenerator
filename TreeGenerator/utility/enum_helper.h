#ifndef TREE_GENERATOR_UTILITY_ENUM_HELPER_H_
#define TREE_GENERATOR_UTILITY_ENUM_HELPER_H_

#include <iterator>
#include <type_traits>

namespace tree_generator::utility
{
	template <typename TEnum>
	constexpr TEnum NextEnumValue(TEnum Value) {
		return static_cast<TEnum>(
			static_cast<std::underlying_type_t<TEnum>>(Value) + 1);
	}

	// Provides an interface for iterating over enum values.
	// 
	// Caution: This will iterate over all values in the range Start to End,
	// even if they do not correspond to a "named" value!
	template <typename TEnum, TEnum Start, TEnum End>
	class EnumIterator
	{
	public:
		class iterator
		{
		public:
			using iterator_category = std::input_iterator_tag;
			using value_type = TEnum;
			using difference_type = std::underlying_type_t<TEnum>;
			using pointer = const TEnum*;
			using reference = TEnum;

			explicit iterator(TEnum value) : value_(value)
			{
			}

			iterator& operator++()
			{
				value_ = NextEnumValue<TEnum>(value_);
				return *this;
			}

			iterator operator++(int)
			{
				iterator retval = *this;
				++(*this);
				return retval;
			}

			bool operator==(iterator other) const
			{
				return value_ == other.value_;
			}

			bool operator!=(iterator other) const
			{
				return value_ != other.value_;
			}

			reference operator*() const
			{
				return value_;
			}

		private:
			TEnum value_;
		};

		iterator begin()
		{
			return iterator(Start);
		}

		iterator end()
		{
			return iterator(End);
		}
	};

	template <typename TEnum, TEnum End>
	using EnumIteratorFromZero = EnumIterator<TEnum, static_cast<TEnum>(0), End>;

	template <typename TEnum, TEnum First, TEnum Last>
	using EnumIteratorInclusive = EnumIterator<
		TEnum,
		First,
		NextEnumValue<TEnum>(Last)>;

	template <typename TEnum, TEnum Last>
	using EnumIteratorFromZeroInclusive =
		EnumIteratorFromZero<TEnum, NextEnumValue<TEnum>(Last)>;
}

#endif // !TREE_GENERATOR_UTILITY_ENUM_HELPER_H_
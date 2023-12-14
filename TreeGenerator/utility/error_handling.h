#ifndef TREE_GENERATOR_UTILITY_ERROR_HANDLING_H_
#define TREE_GENERATOR_UTILITY_ERROR_HANDLING_H_

#include <memory>
#include <stdexcept>
#include <string>

namespace tree_generator::utility
{
	template <typename T>
	std::unique_ptr<T> ThrowIfNull(std::unique_ptr<T> value, const std::string& message)
	{
		if (value == nullptr)
		{
			throw std::runtime_error(message);
		}
		return std::move(value);
	}
}

#endif // !TREE_GENERATOR_UTILITY_ERROR_HANDLING_H_
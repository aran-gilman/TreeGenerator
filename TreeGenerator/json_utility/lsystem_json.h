#ifndef TREE_GENERATOR_JSON_UTILITY_LSYSTEM_JSON_H_
#define TREE_GENERATOR_JSON_UTILITY_LSYSTEM_JSON_H_

#include <nlohmann/json.hpp>

#include "../lsystem/core/lsystem.h"

namespace tree_generator::lsystem
{
	void to_json(nlohmann::json& j, const LSystem& lSystem);
}

#endif  // !TREE_GENERATOR_JSON_UTILITY_LSYSTEM_JSON_H_
#include "lsystem_json.h"

#include <map>

#include "../lsystem/core/lsystem_parser.h"

namespace tree_generator::lsystem
{
	void to_json(nlohmann::json& j, const LSystem& lSystem)
	{
		j.emplace("axiom", ToString(lSystem.axiom));

		std::map<std::string, std::string> stringMap;
		for (const auto& [key, value] : lSystem.rules)
		{
			stringMap.emplace(ToString(key), ToString(value));
		}
		j.emplace("rules", stringMap);
	}

	void from_json(const nlohmann::json& j, LSystem& lSystem)
	{
		StringLSystem stringLSystem;
		j.at("axiom").get_to(stringLSystem.axiom);

		auto& rules = j.at("rules");
		for (const auto& [symbol, action] : rules.items())
		{
			stringLSystem.rules.push_back(std::make_pair(symbol, action));
		}
		lSystem = ParseLSystem(stringLSystem);
	}
}

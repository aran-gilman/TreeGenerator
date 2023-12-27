#include "lsystem_json.h"

#include <map>

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

	}
}

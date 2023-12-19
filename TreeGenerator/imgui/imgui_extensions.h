#ifndef TREE_GENERATOR_IMGUI_EXTENSIONS_H_
#define TREE_GENERATOR_IMGUI_EXTENSIONS_H_

#include <string>

#include "../utility/enum_helper.h"

using ::tree_generator::utility::EnumIterator;

namespace tree_generator::imgui
{
	template<typename TEnum, TEnum Start, TEnum End>
	TEnum EnumCombo(
		const std::string& label,
		TEnum currentValue,
		EnumIterator<TEnum, Start, End> iterator)
	{
		TEnum newValue = currentValue;
		if (ImGui::BeginCombo(label.c_str(), GetName(currentValue).c_str()))
		{
			for (TEnum value : iterator)
			{
				const bool isSelected = (value == currentValue);
				if (ImGui::Selectable(GetName(type).c_str(), isSelected))
				{
					newValue = value;
				}
			}
			ImGui::EndCombo();
		}
		return newValue;
	}
}

#endif // !TREE_GENERATOR_IMGUI_EXTENSIONS_H_
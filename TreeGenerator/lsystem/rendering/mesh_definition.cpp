#include "mesh_definition.h"

#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

namespace tree_generator::lsystem
{
	CylinderDefinition::CylinderDefinition(int sideCount, float height, float radius) :
		sideCount_(sideCount),
		height_(height),
		radius_(radius)
	{

	}

	bool CylinderDefinition::ShowGUI()
	{
		bool wasChanged = false;
		if (ImGui::InputInt("Side Count", &sideCount_))
		{
			if (sideCount_ < 3)
			{
				sideCount_ = 3;
			}
			else
			{
				wasChanged = true;
			}
		}

		float previousHeight = height_;
		if (ImGui::InputFloat("Height", &height_, 0.01f, 0.1f))
		{
			if (height_ < 0.0f)
			{
				height_ = 0.0f;
			}

			if (glm::abs(previousHeight - height_) > 0.00001f)
			{
				wasChanged = true;
			}
		}

		float previousRadius = radius_;
		if (ImGui::InputFloat("Radius", &radius_, 0.01f, 0.1f))
		{
			if (radius_ < 0.0f)
			{
				radius_ = 0.0f;
			}

			if (glm::abs(previousRadius - radius_) > 0.00001f)
			{
				wasChanged = true;
			}
		}
		return wasChanged;
	}

	MeshData CylinderDefinition::GenerateMesh() const
	{
		return CreateCylinder(sideCount_, height_, radius_);
	}

	QuadDefinition::QuadDefinition() :
		width_(1.0f),
		height_(1.0f),
		skew_(0.0f)
	{
	}

	bool QuadDefinition::ShowGUI()
	{
		bool wasChanged = false;
		wasChanged |= ImGui::InputFloat("Width", &width_, 0.01f, 0.1f);
		wasChanged |= ImGui::InputFloat("Height", &height_, 0.01f, 0.1f);
		wasChanged |= ImGui::InputFloat("Skew", &skew_, 1.0f, 10.0f);
		return wasChanged;
	}

	MeshData QuadDefinition::GenerateMesh() const
	{
		float x = width_ / 2;
		float y = height_ / 2;

		float skewX = glm::sin(glm::radians(skew_)) * height_ / 2;
		float skewY = glm::cos(glm::radians(skew_)) * height_ / 2;

		glm::vec2 bottomLeft(-x - skewX, -skewY);
		glm::vec2 topLeft(-x + skewX, skewY);
		glm::vec2 bottomRight(x - skewX, -skewY);
		glm::vec2 topRight(x + skewX, skewY);

		return CreateQuad(bottomLeft, topLeft, bottomRight, topRight);
	}
}
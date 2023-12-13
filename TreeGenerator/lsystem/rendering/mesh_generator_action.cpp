#include "mesh_generator_action.h"

#include <stdexcept>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <imgui.h>

namespace tree_generator::lsystem
{
	namespace
	{
		Transform CreateTransform(const MeshGeneratorState& state)
		{
			return Transform { 
				state.positionStack.back(), 
				state.rotationStack.back(), 
				1.0f };
		}
	}

	void MeshGeneratorAction::ShowGUI()
	{
		ImGui::Text("<No options>");
	}

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

	DrawAction::DrawAction(
		std::unique_ptr<MeshDefinition> meshDefinition, Material material) :
		meshDefinition_(std::move(meshDefinition)),
		material_(std::move(material))
	{
		if (meshDefinition_ == nullptr)
		{
			throw std::invalid_argument(
				"Failed to initialize DrawAction: meshDefinition must be non-null");
		}
		meshData_ = meshDefinition_->GenerateMesh();
	}

	void DrawAction::PerformAction(const Symbol& symbol, MeshGeneratorState* state)
	{
		if (auto iter = state->symbolMeshMap.find(symbol);
			iter != state->symbolMeshMap.end())
		{
			iter->second.instances.push_back(CreateTransform(*state));
		}
		else
		{
			state->symbolMeshMap.emplace(symbol,
				MeshGroup{ meshData_, { CreateTransform(*state) }, material_ });
		}
	}

	void DrawAction::ShowGUI()
	{
		if (meshDefinition_->ShowGUI())
		{
			meshData_ = meshDefinition_->GenerateMesh();
		}
		ImGui::ColorEdit4("Material color", glm::value_ptr(material_.color));
	}

	MoveAction::MoveAction(float distance) : distance_(distance) {}

	void MoveAction::PerformAction(const Symbol& symbol, MeshGeneratorState* state)
	{
		glm::vec3 rotation = state->rotationStack.back();

		glm::mat4 matrix = glm::mat4(1.0f);
		matrix = glm::rotate(matrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
		matrix = glm::rotate(matrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		matrix = glm::rotate(matrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));

		glm::vec3 direction = glm::vec3(0.0f, 1.0f, 0.0f);
		direction = glm::mat3(matrix) * direction;

		state->positionStack.back() += (direction * distance_);
	}

	void MoveAction::ShowGUI()
	{
		ImGui::InputFloat("Distance", &distance_, 0.01f, 0.1f);
	}

	RotateAction::RotateAction(glm::vec3 rotation) : rotation_(rotation) {}

	void RotateAction::PerformAction(const Symbol& symbol, MeshGeneratorState* state)
	{
		state->rotationStack.back() += rotation_;
	}

	void RotateAction::ShowGUI()
	{
		ImGui::InputFloat3("Angles (degrees)", &rotation_.x);
	}

	void SaveAction::PerformAction(const Symbol& symbol, MeshGeneratorState* state)
	{
		state->positionStack.push_back(state->positionStack.back());
		state->rotationStack.push_back(state->rotationStack.back());
	}

	void RestoreAction::PerformAction(const Symbol& symbol, MeshGeneratorState* state)
	{
		state->positionStack.pop_back();
		state->rotationStack.pop_back();
	}
}

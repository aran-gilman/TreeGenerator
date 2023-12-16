#ifndef TREE_GENERATOR_LSYSTEM_MESH_GENERATOR_ACTION_H_
#define TREE_GENERATOR_LSYSTEM_MESH_GENERATOR_ACTION_H_

#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include <glm/glm.hpp>

#include "../core/lsystem.h"
#include "../../graphics/common/material.h"
#include "../../graphics/common/mesh_data.h"
#include "../../graphics/common/transform.h"
#include "../../utility/enum_helper.h"

namespace tree_generator::lsystem
{
	class MeshDefinition;

	// Output type of the mesh generator.
	struct MeshGroup
	{
		MeshData mesh;
		std::vector<Transform> instances;
		Material material;
	};

	// State of the mesh generator during construction of the MeshGroups.
	// 
	// In the long run, this should be replaced with some other interface/type
	// that allows for more flexible composition of state data based on the
	// attached actions and while also better encapsulating implementation
	// details.
	struct MeshGeneratorState
	{
		std::vector<glm::vec3> positionStack;
		std::vector<glm::vec3> rotationStack;
		std::unordered_map<Symbol, MeshGroup> symbolMeshMap;
	};

	// Interface for mutating the generator state.
	class MeshGeneratorAction
	{
	public:
		enum class ActionType
		{
			None,
			Draw,
			Move,
			Rotate,
			Save,
			Restore
		};
		using ActionTypeIterator = utility::EnumIteratorInclusive<
			ActionType,
			ActionType::None,
			ActionType::Restore>;

		static std::string GetActionTypeName(ActionType actionType);

		virtual ~MeshGeneratorAction() {}
		virtual void PerformAction(const Symbol& symbol, MeshGeneratorState* state) = 0;

		virtual void ShowGUI();
		virtual const std::string_view Name() const = 0;
	};

	// Render a mesh to the screen.
	class DrawAction : public MeshGeneratorAction
	{
	public:
		DrawAction(std::unique_ptr<MeshDefinition> meshDefinition, Material material);
		void PerformAction(const Symbol& symbol, MeshGeneratorState* state) override;

		void ShowGUI() override;
		const std::string_view Name() const override;

	private:
		std::unique_ptr<MeshDefinition> meshDefinition_;
		MeshData meshData_;
		Material material_;
	};

	// Move the generator's turtle position forward.
	class MoveAction : public MeshGeneratorAction
	{
	public:
		MoveAction(float distance);
		MoveAction() : MoveAction(1.0f) {}

		void PerformAction(const Symbol& symbol, MeshGeneratorState* state) override;

		void ShowGUI() override;
		const std::string_view Name() const override { return kName_; }

	private:
		inline static const std::string kName_ = "Move forward";
		float distance_;
	};

	// Rotate the model and change the turtle's direction.
	class RotateAction : public MeshGeneratorAction
	{
	public:
		RotateAction(glm::vec3 rotation);
		void PerformAction(const Symbol& symbol, MeshGeneratorState* state) override;

		void ShowGUI() override;
		const std::string_view Name() const override { return kName_; }

	private:
		inline static const std::string kName_ = "Rotate";
		glm::vec3 rotation_;
	};

	// Save the current state to the stack.
	class SaveAction : public MeshGeneratorAction
	{
	public:
		void PerformAction(const Symbol& symbol, MeshGeneratorState* state) override;
		const std::string_view Name() const override { return kName_; }

	private:
		inline static const std::string kName_ = "Push state";
	};

	// Pop the current state from the stack, returning to the previous one.
	class RestoreAction : public MeshGeneratorAction
	{
	public:
		void PerformAction(const Symbol& symbol, MeshGeneratorState* state) override;
		const std::string_view Name() const override { return kName_; }

	private:
		inline static const std::string kName_ = "Pop state";
	};
}

#endif  // !TREE_GENERATOR_LSYSTEM_MESH_GENERATOR_ACTION_H_
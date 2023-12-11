#ifndef TREE_GENERATOR_LSYSTEM_MESH_GENERATOR_ACTION_H_
#define TREE_GENERATOR_LSYSTEM_MESH_GENERATOR_ACTION_H_

#include <unordered_map>
#include <vector>

#include <glm/glm.hpp>

#include "../core/lsystem.h"
#include "../../graphics/common/mesh_data.h"
#include "../../graphics/common/transform.h"

namespace tree_generator::lsystem
{
	// Output type of the mesh generator.
	// The fields can be passed to the graphics library.
	struct MeshGroup
	{
		MeshData mesh;
		std::vector<Transform> instances;
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
		virtual ~MeshGeneratorAction() {}
		virtual void PerformAction(const Symbol& symbol, MeshGeneratorState* state) = 0;
	};

	// Render a mesh to the screen.
	class DrawAction : public MeshGeneratorAction
	{
	public:
		DrawAction(MeshData meshData);
		void PerformAction(const Symbol& symbol, MeshGeneratorState* state) override;

	private:
		MeshData meshData_;
	};

	// Move the generator's turtle position forward.
	class MoveAction : public MeshGeneratorAction
	{
	public:
		void PerformAction(const Symbol& symbol, MeshGeneratorState* state) override;
	};

	// Rotate the model and change the turtle's direction.
	class RotateAction : public MeshGeneratorAction
	{
	public:
		RotateAction(glm::vec3 rotation);
		void PerformAction(const Symbol& symbol, MeshGeneratorState* state) override;

	private:
		glm::vec3 rotation_;
	};

	// Save the current state to the stack.
	class SaveAction : public MeshGeneratorAction
	{
	public:
		void PerformAction(const Symbol& symbol, MeshGeneratorState* state) override;
	};

	// Pop the current state from the stack, returning to the previous one.
	class RestoreAction : public MeshGeneratorAction
	{
	public:
		void PerformAction(const Symbol& symbol, MeshGeneratorState* state) override;
	};
}

#endif  // !TREE_GENERATOR_LSYSTEM_MESH_GENERATOR_ACTION_H_
#ifndef TREE_GENERATOR_RENDERER_H_
#define TREE_GENERATOR_RENDERER_H_

#include <vector>

#include <glm/glm.hpp>

#include "mesh_data.h"
#include "transform.h"

namespace tree_generator
{
	// Draws objects to the screen
	class Renderer
	{
	public:
		virtual ~Renderer() = default;

		virtual void SetCameraView(glm::mat4 view) = 0;
		virtual void SetWindowFramebufferSize(int width, int height) = 0;

		virtual void AddMesh(const MeshData& meshData) = 0;
		virtual void AddMesh(const MeshData& meshData, const Transform& transform) = 0;
		virtual void AddMesh(
			const MeshData& meshData, const std::vector<Transform>& instances) = 0;

		virtual void ClearAllMeshes() = 0;

		virtual void Render() = 0;

	protected:
		Renderer() {}
	};
}

#endif  // !TREE_GENERATOR_RENDERER_H_
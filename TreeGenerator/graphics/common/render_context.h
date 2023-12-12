#ifndef TREE_GENERATOR_RENDER_CONTEXT_H_
#define TREE_GENERATOR_RENDER_CONTEXT_H_

#include <vector>

#include <glm/glm.hpp>

#include "mesh_data.h"
#include "transform.h"

namespace tree_generator
{
	// Draws objects to the screen
	class RenderContext
	{
	public:
		virtual ~RenderContext() = default;

		virtual void SetCameraView(glm::mat4 view) = 0;
		virtual void SetWindowFramebufferSize(int width, int height) = 0;

		virtual void AddMesh(const MeshData& meshData) = 0;
		virtual void AddMesh(const MeshData& meshData, const Transform& transform) = 0;
		virtual void AddMesh(
			const MeshData& meshData, const std::vector<Transform>& instances) = 0;

		virtual void ClearAllMeshes() = 0;

		virtual void Render() = 0;

	protected:
		RenderContext() {}
	};
}

#endif  // !TREE_GENERATOR_RENDER_CONTEXT_H_
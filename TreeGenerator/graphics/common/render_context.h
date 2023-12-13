#ifndef TREE_GENERATOR_RENDER_CONTEXT_H_
#define TREE_GENERATOR_RENDER_CONTEXT_H_

#include <memory>
#include <vector>

#include <glm/glm.hpp>

#include "mesh_data.h"
#include "transform.h"

namespace tree_generator
{
	class Camera;
	class MeshRenderer;

	// Draws objects to the screen
	class RenderContext
	{
	public:
		virtual ~RenderContext() = default;

		virtual std::unique_ptr<Camera> CreateCamera() = 0;
		virtual std::unique_ptr<MeshRenderer> CreateMeshRenderer() = 0;

	protected:
		RenderContext() {}
	};
}

#endif  // !TREE_GENERATOR_RENDER_CONTEXT_H_
#ifndef TREE_GENERATOR_RENDER_CONTEXT_H_
#define TREE_GENERATOR_RENDER_CONTEXT_H_

#include <memory>

namespace tree_generator
{
	class Camera;
	class MeshRenderer;

	// Manages the backend graphics library (e.g. OpenGL, Vulkan, etc) and the
	// creation of classes that need to interact with it.
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
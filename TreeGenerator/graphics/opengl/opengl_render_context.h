#ifndef TREE_GENERATOR_OPENGL_RENDER_CONTEXT_H_
#define TREE_GENERATOR_OPENGL_RENDER_CONTEXT_H_

#include <memory>

#include "../common/render_context.h"

namespace tree_generator
{
	class Camera;
	class MeshRenderer;
	class Window;
}

namespace tree_generator::opengl
{
	class ShaderProgram;

	class OpenGLRenderContext : public RenderContext
	{
	public:
		OpenGLRenderContext();
		~OpenGLRenderContext() override;

		// Warning: Multiple cameras are currently not supported!
		// Creating a new camera overwrites the old camera
		std::unique_ptr<Camera> CreateCamera() override;

		std::unique_ptr<MeshRenderer> CreateMeshRenderer() override;

	private:
		std::unique_ptr<ShaderProgram> normalShader_;
		std::unique_ptr<ShaderProgram> materialShader_;
	};
}

#endif  // !TREE_GENERATOR_OPENGL_RENDER_CONTEXT_H_
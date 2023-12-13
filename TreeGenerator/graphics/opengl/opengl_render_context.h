#ifndef TREE_GENERATOR_OPENGL_RENDER_CONTEXT_H_
#define TREE_GENERATOR_OPENGL_RENDER_CONTEXT_H_

#include <memory>

#include <glm/glm.hpp>

#include "../common/render_context.h"
#include "../common/transform.h"
#include "opengl_mesh_renderer.h"

namespace tree_generator
{
	class Camera;
	class Window;

	namespace opengl
	{
		class ShaderProgram;

		class OpenGLRenderContext : public RenderContext
		{
		public:
			OpenGLRenderContext(Window* window);
			~OpenGLRenderContext() override;

			// Warning: Multiple cameras are currently not supported!
			// Creating a new camera overwrites the old camera
			std::unique_ptr<Camera> CreateCamera() override;

			void AddMesh(const MeshData& meshData) override;
			void AddMesh(const MeshData& meshData, const Transform& transform) override;
			void AddMesh(
				const MeshData& meshData, const std::vector<Transform>& instances) override;

			void ClearAllMeshes() override;

			void Render() override;

		private:
			std::unique_ptr<ShaderProgram> normalShader_;
			std::unique_ptr<ShaderProgram> materialShader_;

			std::vector<MeshRenderData> meshRenderData;
		};
	}
}

#endif  // !TREE_GENERATOR_OPENGL_RENDER_CONTEXT_H_
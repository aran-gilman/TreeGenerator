#ifndef TREE_GENERATOR_OPENGL_RENDERER_H_
#define TREE_GENERATOR_OPENGL_RENDERER_H_

#include <glm/glm.hpp>

#include "../common/mesh_data.h"
#include "../common/renderer.h"
#include "../common/transform.h"

namespace tree_generator
{
	class Window;

	namespace opengl
	{
		struct MeshRenderData
		{
			MeshData meshData;

			unsigned int vertexArray;
			unsigned int vertexBuffer;
			unsigned int indexBuffer;

			unsigned int instanceTransformBuffer;
			int instanceCount;
		};

		class OpenGLRenderer : public Renderer
		{
		public:
			OpenGLRenderer(Window* window);
			~OpenGLRenderer() override;

			void SetCameraView(glm::mat4 view) override;
			void SetWindowFramebufferSize(int width, int height) override;

			void AddMesh(const MeshData& meshData) override;
			void AddMesh(const MeshData& meshData, const Transform& transform) override;
			void AddMesh(
				const MeshData& meshData, const std::vector<Transform>& instances) override;

			void ClearAllMeshes() override;

			void Render() override;

		private:
			unsigned int normalShader_;
			unsigned int cameraBuffer;
			std::vector<MeshRenderData> meshRenderData;
		};
	}
}

#endif  // !TREE_GENERATOR_OPENGL_RENDERER_H_
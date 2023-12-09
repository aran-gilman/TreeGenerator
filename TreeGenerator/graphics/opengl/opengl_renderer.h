#ifndef TREE_GENERATOR_OPENGL_RENDERER_H_
#define TREE_GENERATOR_OPENGL_RENDERER_H_

#include "../mesh_data.h"
#include "../renderer.h"

namespace tree_generator
{
	namespace opengl
	{
		struct MeshRenderData
		{
			MeshData meshData;
			unsigned int vbo;
			unsigned int ebo;
			unsigned int vao;
		};

		class OpenGLRenderer : public Renderer
		{
		public:
			OpenGLRenderer();
			~OpenGLRenderer() override;

			void AddMesh(const MeshData& meshData) override;
			void Render() override;

		private:
			unsigned int shader;
			std::vector<MeshRenderData> meshRenderData;
		};
	}
}

#endif  // !TREE_GENERATOR_OPENGL_RENDERER_H_
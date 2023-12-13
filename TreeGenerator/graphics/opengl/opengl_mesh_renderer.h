#ifndef TREE_GENERATOR_OPENGL_MESH_RENDERER_H_
#define TREE_GENERATOR_OPENGL_MESH_RENDERER_H_

#include "../common/material.h"
#include "../common/mesh_data.h"
#include "../common/mesh_renderer.h"

namespace tree_generator::opengl
{
	struct MeshRenderData
	{
		MeshData meshData;
		Material material;

		unsigned int vertexArray;
		unsigned int vertexBuffer;
		unsigned int indexBuffer;

		unsigned int instanceTransformBuffer;
		int instanceCount;
	};

	class OpenGLMeshRenderer : public MeshRenderer
	{
	public:
		void Render() override;
	};
}

#endif // !TREE_GENERATOR_OPENGL_MESH_RENDERER_H_
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
		OpenGLMeshRenderer();
		~OpenGLMeshRenderer();

		void SetMeshData(const MeshData& meshData) override;
		void SetMeshData(const MeshData& meshData, const Transform& instance) override;
		void SetMeshData(
			const MeshData& meshData, const std::vector<Transform>& instances) override;

		void Render() override;

	private:
		unsigned int vertexArray_;
		unsigned int vertexBuffer_;
		unsigned int indexBuffer_;
		unsigned int instanceTransformBuffer_;

		int indexCount_;
		int instanceCount_;
	};
}

#endif // !TREE_GENERATOR_OPENGL_MESH_RENDERER_H_
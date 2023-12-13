#ifndef TREE_GENERATOR_MESH_RENDERER_H_
#define TREE_GENERATOR_MESH_RENDERER_H_

#include <vector>

#include "material.h"
#include "mesh_data.h"
#include "transform.h"

namespace tree_generator
{
	class MeshRenderer
	{
	public:
		enum class RenderMode
		{
			Material,
			Normals
		};

		virtual ~MeshRenderer() = default;

		virtual void SetMeshData(const MeshData& meshData) = 0;
		virtual void SetMeshData(const MeshData& meshData, const Transform& instance) = 0;
		virtual void SetMeshData(
			const MeshData& meshData, const std::vector<Transform>& instances) = 0;

		virtual void SetMaterial(Material material) = 0;

		virtual void Render(RenderMode mode) = 0;

	protected:
		MeshRenderer() {}
	};
}

#endif // !TREE_GENERATOR_MESH_RENDERER_H_
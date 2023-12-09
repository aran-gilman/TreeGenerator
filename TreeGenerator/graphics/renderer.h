#ifndef TREE_GENERATOR_RENDERER_H_
#define TREE_GENERATOR_RENDERER_H_

#include "mesh_data.h"

namespace tree_generator
{
	class Renderer
	{
	public:
		virtual ~Renderer() = default;

		virtual void AddMesh(const MeshData& meshData) = 0;
		virtual void Render() = 0;

	protected:
		Renderer() {}
	};
}

#endif  // !TREE_GENERATOR_RENDERER_H_
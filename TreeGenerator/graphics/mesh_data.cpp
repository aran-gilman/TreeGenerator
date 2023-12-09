#include "mesh_data.h"

namespace tree_generator
{
	MeshData CreateQuad()
	{
		MeshData quad;
		quad.vertices = {
			{{  0.5f,  0.5f, 0.0f}},
			{{  0.5f, -0.5f, 0.0f}},
			{{ -0.5f, -0.5f, 0.0f}},
			{{ -0.5f,  0.5f, 0.0f}}
		};
		quad.indices = {
			0, 1, 3,
			1, 2, 3
		};
		return quad;
	}
}

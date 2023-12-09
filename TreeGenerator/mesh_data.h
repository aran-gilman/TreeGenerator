#ifndef TREE_GENERATOR_MESH_DATA_H_
#define TREE_GENERATOR_MESH_DATA_H_

#include <vector>

#include <glm/glm.hpp>

namespace tree_generator
{
	struct MeshData
	{
		std::vector<glm::vec3> vertices;
		std::vector<unsigned int> indices;
	};

	MeshData CreateQuad();
}

#endif  // !TREE_GENERATOR_MESH_DATA_H_
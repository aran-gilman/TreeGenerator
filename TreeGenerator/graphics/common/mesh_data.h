#ifndef TREE_GENERATOR_MESH_DATA_H_
#define TREE_GENERATOR_MESH_DATA_H_

#include <vector>

#include <glm/glm.hpp>

namespace tree_generator
{
	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec3 uv;
	};

	struct MeshData
	{
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
	};

	MeshData CreateQuad();
}

#endif  // !TREE_GENERATOR_MESH_DATA_H_
#include "mesh_data.h"

#include <iostream>

#include <glm/gtc/constants.hpp>

namespace tree_generator
{
	namespace
	{
		std::vector<glm::vec2> CalculateCirclePositions(int sideCount)
		{
			float sideStep = glm::two_pi<float>() / sideCount;
			std::vector<glm::vec2> positions;
			for (float angle = 0.0f; angle < glm::two_pi<float>(); angle += sideStep)
			{
				positions.emplace_back(glm::cos(angle), glm::sin(angle));
			}
			return positions;
		}
	}

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

	// Reference: http://www.songho.ca/opengl/gl_cylinder.html
	MeshData CreateCylinder(int sideCount)
	{
		if (sideCount < 3)
		{
			std::cerr << "Failed to generate cylinder: must have at least 3 sides" << std::endl;
			return {};
		}

		std::vector<glm::vec2> circle = CalculateCirclePositions(sideCount);
		MeshData mesh;
		for (int i = 0; i < circle.size(); ++i)
		{
			mesh.vertices.push_back({ glm::vec3(circle[i], -0.5f) });
			mesh.vertices.push_back({ glm::vec3(circle[i], 0.5f) });

			if (i < circle.size() - 1)
			{
				mesh.indices.push_back(i + 2);
				mesh.indices.push_back(i + 3);
				mesh.indices.push_back(i + 1);

				mesh.indices.push_back(i + 3);
				mesh.indices.push_back(i);
				mesh.indices.push_back(i + 1);
			}
			else
			{
				mesh.indices.push_back(0);
				mesh.indices.push_back(1);
				mesh.indices.push_back(i + 1);

				mesh.indices.push_back(0);
				mesh.indices.push_back(i);
				mesh.indices.push_back(i + 1);
			}
		}
		return mesh;
	}
}

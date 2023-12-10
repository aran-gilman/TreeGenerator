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
		glm::vec3 normal(0.0f, 0.0f, -1.0f);

		MeshData quad;
		quad.vertices = {
			{{  0.5f,  0.5f, 0.0f}, normal},
			{{  0.5f, -0.5f, 0.0f}, normal},
			{{ -0.5f, -0.5f, 0.0f}, normal},
			{{ -0.5f,  0.5f, 0.0f}, normal}
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
			glm::vec3 normal = glm::vec3(circle[i], 0.0f);
			mesh.vertices.push_back({ glm::vec3(circle[i], -0.5f), normal });
			mesh.vertices.push_back({ glm::vec3(circle[i], 0.5f), normal });

			if (i < circle.size() - 1)
			{
				mesh.indices.push_back(2 * i + 3);
				mesh.indices.push_back(2 * i + 2);
				mesh.indices.push_back(2 * i + 1);

				mesh.indices.push_back(2 * i + 2);
				mesh.indices.push_back(2 * i);
				mesh.indices.push_back(2 * i + 1);
			}
			else
			{
				mesh.indices.push_back(0);
				mesh.indices.push_back(1);
				mesh.indices.push_back(2 * i + 1);

				mesh.indices.push_back(0);
				mesh.indices.push_back(2 * i);
				mesh.indices.push_back(2 * i + 1);
			}
		}
		return mesh;
	}
}

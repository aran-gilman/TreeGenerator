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

	MeshData CreateQuad(
		glm::vec2 bottomLeft,
		glm::vec2 topLeft,
		glm::vec2 bottomRight,
		glm::vec2 topRight)
	{
		glm::vec3 normal(0.0f, 0.0f, -1.0f);

		MeshData quad;
		quad.vertices = {
			{ glm::vec3(topRight, 0.0f), normal},
			{ glm::vec3(bottomRight, 0.0f), normal},
			{ glm::vec3(bottomLeft, 0.0f), normal},
			{ glm::vec3(topLeft, 0.0f), normal}
		};
		quad.indices = {
			0, 1, 3,
			1, 2, 3
		};
		return quad;
	}

	// Reference: http://www.songho.ca/opengl/gl_cylinder.html
	MeshData CreateCylinder(int sideCount, float height, float radius)
	{
		if (sideCount < 3)
		{
			std::cerr << "Failed to generate cylinder: must have at least 3 sides" << std::endl;
			return {};
		}

		std::vector<glm::vec2> circle = CalculateCirclePositions(sideCount);
		MeshData mesh;
		glm::vec3 offset(0.0f, height / 2, 0.0f);
		for (int i = 0; i < circle.size(); ++i)
		{
			glm::vec3 normal = glm::vec3(circle[i].x, 0, circle[i].y);
			glm::vec3 basePosition = normal * radius;
			mesh.vertices.push_back({ basePosition - offset, normal });
			mesh.vertices.push_back({ basePosition + offset, normal });

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

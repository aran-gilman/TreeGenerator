#include <cstdlib>
#include <iostream>
#include <memory>
#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "Window.h"

using ::tree_generator::Window;

namespace
{
	const char* vertexShaderSource = R"s(
#version 330 core
layout (location = 0) in vec3 aPos;

void main()
{
	gl_Position = vec4(aPos, 1.0f);
})s";

	const char* fragmentShaderSource = R"s(
#version 330 core

out vec4 FragColor;

void main()
{
	FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
})s";

	struct MeshData
	{
		std::vector<glm::vec3> vertices;
		std::vector<unsigned int> indices;
	};

	struct MeshRenderData
	{
		MeshData meshData;
		unsigned int vbo;
		unsigned int ebo;
		unsigned int vao;
	};

	MeshData CreateQuad()
	{
		MeshData quad;
		quad.vertices = {
			{0.5f, 0.5f, 0.0f},
			{0.5f, -0.5f, 0.0f},
			{-0.5f, -0.5f, 0.0f},
			{-0.5f, 0.5f, 0.0f}
		};
		quad.indices = {
			0, 1, 3,
			1, 2, 3
		};
		return quad;
	}
}

int main()
{
	try
	{
		std::unique_ptr<Window> window = Window::Create(800, 600, "TreeGenerator");

		MeshRenderData quad;
		quad.meshData = CreateQuad();
		glGenBuffers(1, &quad.vbo);
		glGenBuffers(1, &quad.ebo);
		glGenVertexArrays(1, &quad.vao);

		glBindVertexArray(quad.vao);

		glBindBuffer(GL_ARRAY_BUFFER, quad.vbo);
		glBufferData(
			GL_ARRAY_BUFFER,
			sizeof(glm::vec3) * quad.meshData.vertices.size(),
			quad.meshData.vertices.data(),
			GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quad.ebo);
		glBufferData(
			GL_ELEMENT_ARRAY_BUFFER,
			sizeof(unsigned int) * quad.meshData.indices.size(),
			quad.meshData.indices.data(),
			GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
		glCompileShader(vertexShader);

		int success;
		char infoLog[512];
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
			std::cerr << "Vertex shader compilation failed: " << infoLog << std::endl;
		}

		unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
		glCompileShader(fragmentShader);
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
			std::cerr << "Fragment shader compilation failed: " << infoLog << std::endl;
		}

		unsigned int shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		glLinkProgram(shaderProgram);
		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
			std::cerr << "Shader program linking failed: " << infoLog << std::endl;
		}

		window->Display([&]() {
			glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			glUseProgram(shaderProgram);
			glDrawElements(GL_TRIANGLES, quad.meshData.indices.size(), GL_UNSIGNED_INT, 0);
			});

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		glDeleteProgram(shaderProgram);

		glDeleteVertexArrays(1, &quad.vao);
		glDeleteBuffers(1, &quad.vbo);
		glDeleteBuffers(1, &quad.ebo);
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

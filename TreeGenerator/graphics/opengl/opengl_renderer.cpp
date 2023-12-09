#include "opengl_renderer.h"

#include <iostream>

#include <glad/glad.h>

namespace tree_generator
{
	namespace opengl
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

		OpenGLRenderer::OpenGLRenderer()
		{
			int success;
			char infoLog[512];

			unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
			glCompileShader(vertexShader);

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

			shader = glCreateProgram();
			glAttachShader(shader, vertexShader);
			glAttachShader(shader, fragmentShader);
			glLinkProgram(shader);

			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(shader, 512, nullptr, infoLog);
				std::cerr << "Shader program linking failed: " << infoLog << std::endl;
			}

			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			glUseProgram(shader);
		}

		OpenGLRenderer::~OpenGLRenderer()
		{
			glDeleteProgram(shader);

			for (const MeshRenderData& mesh : meshRenderData)
			{
				glDeleteVertexArrays(1, &mesh.vao);
				glDeleteBuffers(1, &mesh.vbo);
				glDeleteBuffers(1, &mesh.ebo);
			}
		}

		void OpenGLRenderer::AddMesh(const MeshData& meshData)
		{
			meshRenderData.push_back({});
			MeshRenderData& mesh = meshRenderData.back();

			mesh.meshData = ::tree_generator::CreateQuad();
			glGenBuffers(1, &mesh.vbo);
			glGenBuffers(1, &mesh.ebo);
			glGenVertexArrays(1, &mesh.vao);

			glBindVertexArray(mesh.vao);

			glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
			glBufferData(
				GL_ARRAY_BUFFER,
				sizeof(glm::vec3) * mesh.meshData.vertices.size(),
				mesh.meshData.vertices.data(),
				GL_STATIC_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ebo);
			glBufferData(
				GL_ELEMENT_ARRAY_BUFFER,
				sizeof(unsigned int) * mesh.meshData.indices.size(),
				mesh.meshData.indices.data(),
				GL_STATIC_DRAW);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);
		}

		void OpenGLRenderer::Render()
		{
			glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			for (const MeshRenderData& mesh : meshRenderData)
			{
				glDrawElements(
					GL_TRIANGLES, mesh.meshData.indices.size(), GL_UNSIGNED_INT, 0);
			}
		}
	}
}

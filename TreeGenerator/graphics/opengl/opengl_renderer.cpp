#include "opengl_renderer.h"
#include "opengl_renderer.h"

#include <iostream>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../common/camera_data.h"
#include "../common/window.h"

namespace tree_generator
{
	namespace opengl
	{
		const char* vertexShaderSource = R"s(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

layout (std140) uniform Camera
{
	mat4 view;
	mat4 projection;
} camera;

out VS_OUT
{
	vec3 normal;
} vs_out;

void main()
{
	gl_Position = camera.projection * camera.view * vec4(aPos, 1.0f);
	vs_out.normal = aNormal;
})s";

		const char* fragmentShaderSource = R"s(
#version 330 core

in VS_OUT
{
	vec3 normal;
} vs_out;

out vec4 FragColor;

void main()
{
	FragColor = vec4(abs(vs_out.normal), 1.0f);
})s";

		OpenGLRenderer::OpenGLRenderer(Window* window)
		{
			int version = gladLoadGL();
			if (version == 0)
			{
				throw std::runtime_error("Failed to load OpenGL");
			}

			glViewport(0, 0, window->Width(), window->Height());
			glEnable(GL_DEPTH_TEST);

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

			CameraData camera{
				glm::lookAt(
					glm::vec3(0.0f, 0.0f, -3.0f),
					glm::vec3(0.0f),
					glm::vec3(0.0f, 1.0f, 0.0f)),
				glm::perspective(
					glm::radians(45.0f),
					(float)window->Width() / window->Height(),
					0.1f, 100.0f)
			};

			glGenBuffers(1, &cameraBuffer);
			glBindBuffer(GL_UNIFORM_BUFFER, cameraBuffer);
			glBufferData(GL_UNIFORM_BUFFER, sizeof(CameraData), &camera, GL_STATIC_DRAW);
			glBindBufferBase(GL_UNIFORM_BUFFER, 1, cameraBuffer);

			unsigned int cameraIndex = glGetUniformBlockIndex(shader, "Camera");
			glUniformBlockBinding(shader, cameraIndex, 1);
		}

		OpenGLRenderer::~OpenGLRenderer()
		{
			glDeleteBuffers(1, &cameraBuffer);
			glDeleteProgram(shader);

			for (const MeshRenderData& mesh : meshRenderData)
			{
				glDeleteVertexArrays(1, &mesh.vao);
				glDeleteBuffers(1, &mesh.vbo);
				glDeleteBuffers(1, &mesh.ebo);
			}
		}

		void OpenGLRenderer::SetCameraPosition(glm::vec3 position)
		{
			glBindBuffer(GL_UNIFORM_BUFFER, cameraBuffer);
			glm::mat4 view =
				glm::lookAt(position, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			glBufferSubData(
				GL_UNIFORM_BUFFER,
				offsetof(CameraData, view),
				sizeof(glm::mat4),
				&view);
		}

		void OpenGLRenderer::AddMesh(const MeshData& meshData)
		{
			meshRenderData.push_back({});
			MeshRenderData& mesh = meshRenderData.back();

			mesh.meshData = meshData;
			glGenBuffers(1, &mesh.vbo);
			glGenBuffers(1, &mesh.ebo);
			glGenVertexArrays(1, &mesh.vao);

			glBindVertexArray(mesh.vao);

			glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
			glBufferData(
				GL_ARRAY_BUFFER,
				sizeof(Vertex) * mesh.meshData.vertices.size(),
				mesh.meshData.vertices.data(),
				GL_STATIC_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ebo);
			glBufferData(
				GL_ELEMENT_ARRAY_BUFFER,
				sizeof(unsigned int) * mesh.meshData.indices.size(),
				mesh.meshData.indices.data(),
				GL_STATIC_DRAW);

			glVertexAttribPointer(
				0, 3,
				GL_FLOAT, GL_FALSE,
				sizeof(Vertex), (void*)offsetof(Vertex, position));
			glEnableVertexAttribArray(0);

			glVertexAttribPointer(
				1, 3,
				GL_FLOAT, GL_FALSE,
				sizeof(Vertex), (void*)offsetof(Vertex, normal));
			glEnableVertexAttribArray(1);
		}

		void OpenGLRenderer::Render()
		{
			glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			for (const MeshRenderData& mesh : meshRenderData)
			{
				glDrawElements(
					GL_TRIANGLES, mesh.meshData.indices.size(), GL_UNSIGNED_INT, 0);
			}
		}
	}
}

#include "opengl_render_context.h"

#include <iostream>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../common/camera_data.h"
#include "../common/window.h"
#include "typed_shader.h"
#include "shader_program.h"

namespace tree_generator
{
	namespace opengl
	{
		const char* vertexShaderSource = R"s(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

// Model matrix (per instance)
layout (location = 3) in vec4 aModel0;
layout (location = 4) in vec4 aModel1;
layout (location = 5) in vec4 aModel2;
layout (location = 6) in vec4 aModel3;

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
	mat4 model = mat4(aModel0, aModel1, aModel2, aModel3);
	gl_Position = camera.projection * camera.view * model * vec4(aPos, 1.0f);
	vs_out.normal = aNormal;
})s";

		const char* normalFragmentShaderSource = R"s(
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
		
		const char* materialFragmentShaderSource = R"s(
#version 330 core

struct Material
{
    vec4 color;
};

uniform Material material;

out vec4 FragColor;

void main()
{
	FragColor = material.color;
})s";
		template <typename T>
		std::unique_ptr<T> ThrowIfNull(std::unique_ptr<T> value, const std::string& message)
		{
			if (value == nullptr)
			{
				throw std::runtime_error(message);
			}
			return std::move(value);
		}

		OpenGLRenderContext::OpenGLRenderContext(Window* window)
		{
			int version = gladLoadGL();
			if (version == 0)
			{
				throw std::runtime_error("Failed to load OpenGL");
			}

			glViewport(0, 0, window->Width(), window->Height());
			glEnable(GL_DEPTH_TEST);

			std::unique_ptr<VertexShader> vertexShader = ThrowIfNull(
				VertexShader::Create(vertexShaderSource),
				"Vertex shader compilation failed");

			std::unique_ptr<FragmentShader> normalFragmentShader = ThrowIfNull(
				FragmentShader::Create(normalFragmentShaderSource),
				"Normal fragment shader compilation failed");

			std::unique_ptr<FragmentShader> materialFragmentShader = ThrowIfNull(
				FragmentShader::Create(materialFragmentShaderSource),
				"Material fragment shader compilation failed");

			normalShader_ = ThrowIfNull(
				ShaderProgram::Create(*vertexShader, *normalFragmentShader),
				"Normal shader linking failed");
			materialShader_ = ThrowIfNull(
				ShaderProgram::Create(*vertexShader, *materialFragmentShader),
				"Material shader linking failed");

			CameraData camera{
				glm::lookAt(
					glm::vec3(0.0f, 0.0f, -3.0f),
					glm::vec3(0.0f),
					glm::vec3(0.0f, 1.0f, 0.0f)),
				glm::perspective(
					glm::radians(45.0f),
					(float)window->Width() / window->Height(),
					0.1f, 1000.0f)
			};

			glGenBuffers(1, &cameraBuffer);
			glBindBuffer(GL_UNIFORM_BUFFER, cameraBuffer);
			glBufferData(GL_UNIFORM_BUFFER, sizeof(CameraData), &camera, GL_STATIC_DRAW);
			glBindBufferBase(GL_UNIFORM_BUFFER, 1, cameraBuffer);

			normalShader_->BindUniformBlock("Camera", 1);
			materialShader_->BindUniformBlock("Camera", 1);
		}

		OpenGLRenderContext::~OpenGLRenderContext()
		{
			glDeleteBuffers(1, &cameraBuffer);

			for (const MeshRenderData& mesh : meshRenderData)
			{
				glDeleteVertexArrays(1, &mesh.vertexArray);
				glDeleteBuffers(1, &mesh.vertexBuffer);
				glDeleteBuffers(1, &mesh.indexBuffer);
				glDeleteBuffers(1, &mesh.instanceTransformBuffer);
			}
		}

		void OpenGLRenderContext::SetCameraView(glm::mat4 view)
		{
			glBindBuffer(GL_UNIFORM_BUFFER, cameraBuffer);
			glBufferSubData(
				GL_UNIFORM_BUFFER,
				offsetof(CameraData, view),
				sizeof(glm::mat4),
				&view);
		}

		void OpenGLRenderContext::SetWindowFramebufferSize(int width, int height)
		{
			glm::mat4 projection = glm::perspective(
				glm::radians(45.0f),
				(float)width / height,
				0.1f, 1000.0f);
			glViewport(0, 0, width, height);
			glBindBuffer(GL_UNIFORM_BUFFER, cameraBuffer);
			glBufferSubData(
				GL_UNIFORM_BUFFER,
				offsetof(CameraData, projection),
				sizeof(glm::mat4),
				&projection);
		}

		void OpenGLRenderContext::AddMesh(const MeshData& meshData)
		{
			AddMesh(meshData, { glm::zero<glm::vec3>(), glm::zero<glm::vec3>(), 1.0f });
		}

		void OpenGLRenderContext::AddMesh(
			const MeshData& meshData, const Transform& transform)
		{
			AddMesh(meshData, std::vector<Transform>({ transform }));
		}

		void OpenGLRenderContext::AddMesh(
			const MeshData& meshData, const std::vector<Transform>& instances)
		{
			meshRenderData.push_back({});
			MeshRenderData& mesh = meshRenderData.back();
			mesh.instanceCount = instances.size();

			mesh.meshData = meshData;
			mesh.material.color = glm::vec4(0.0f, 0.5f, 0.0f, 1.0f);

			glGenBuffers(1, &mesh.vertexBuffer);
			glGenBuffers(1, &mesh.indexBuffer);
			glGenBuffers(1, &mesh.instanceTransformBuffer);

			glGenVertexArrays(1, &mesh.vertexArray);
			glBindVertexArray(mesh.vertexArray);

			glBindBuffer(GL_ARRAY_BUFFER, mesh.vertexBuffer);
			glBufferData(
				GL_ARRAY_BUFFER,
				sizeof(Vertex) * mesh.meshData.vertices.size(),
				mesh.meshData.vertices.data(),
				GL_STATIC_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.indexBuffer);
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

			glVertexAttribPointer(
				2, 2,
				GL_FLOAT, GL_FALSE,
				sizeof(Vertex), (void*)offsetof(Vertex, uv));
			glEnableVertexAttribArray(2);

			// Calculate the model matrices based on the provided transforms.
			// Sending the entire model matrix for each instance over VBO seems
			// feels a bit expensive (4 vec4s, each taking up a binding point).
			// The main alternative would be to calculate the model matrix from
			// the transform within the shader itself, but those calculations
			// are moderately expensive and repeating them every frame feels
			// wasteful.
			std::vector<glm::mat4> modelMatrices;
			modelMatrices.reserve(instances.size());
			for (int i = 0; i < instances.size(); ++i)
			{
				glm::mat4 matrix = glm::mat4(1.0f);

				matrix = glm::translate(matrix, instances[i].position);

				matrix = glm::rotate(
					matrix,
					glm::radians(instances[i].rotation.z),
					glm::vec3(0.0f, 0.0f, 1.0f));
				matrix = glm::rotate(
					matrix,
					glm::radians(instances[i].rotation.y),
					glm::vec3(0.0f, 1.0f, 0.0f));
				matrix = glm::rotate(
					matrix,
					glm::radians(instances[i].rotation.x),
					glm::vec3(1.0f, 0.0f, 0.0f));

				matrix = glm::scale(matrix, glm::vec3(instances[i].scale));

				modelMatrices.push_back(matrix);
			}

			glBindBuffer(GL_ARRAY_BUFFER, mesh.instanceTransformBuffer);
			glBufferData(
				GL_ARRAY_BUFFER,
				sizeof(glm::mat4) * mesh.instanceCount,
				modelMatrices.data(),
				GL_STATIC_DRAW);

			glVertexAttribPointer(
				3, 4,
				GL_FLOAT, GL_FALSE,
				sizeof(glm::mat4), (void*)0);
			glEnableVertexAttribArray(3);
			glVertexAttribDivisor(3, 1);

			glVertexAttribPointer(
				4, 4,
				GL_FLOAT, GL_FALSE,
				sizeof(glm::mat4), (void*)sizeof(glm::vec4));
			glEnableVertexAttribArray(4);
			glVertexAttribDivisor(4, 1);

			glVertexAttribPointer(
				5, 4,
				GL_FLOAT, GL_FALSE,
				sizeof(glm::mat4), (void*)(sizeof(glm::vec4) * 2));
			glEnableVertexAttribArray(5);
			glVertexAttribDivisor(5, 1);

			glVertexAttribPointer(
				6, 4,
				GL_FLOAT, GL_FALSE,
				sizeof(glm::mat4), (void*)(sizeof(glm::vec4) * 3));
			glEnableVertexAttribArray(6);
			glVertexAttribDivisor(6, 1);
		}

		void OpenGLRenderContext::ClearAllMeshes()
		{
			for (const MeshRenderData& mesh : meshRenderData)
			{
				glDeleteVertexArrays(1, &mesh.vertexArray);
				glDeleteBuffers(1, &mesh.vertexBuffer);
				glDeleteBuffers(1, &mesh.indexBuffer);
				glDeleteBuffers(1, &mesh.instanceTransformBuffer);
			}
			meshRenderData.clear();
		}

		void OpenGLRenderContext::Render()
		{
			glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			materialShader_->Bind();

			for (const MeshRenderData& mesh : meshRenderData)
			{
				materialShader_->SetUniform(
					"material.color", mesh.material.color);
				glBindVertexArray(mesh.vertexArray);
				glDrawElementsInstanced(
					GL_TRIANGLES,
					mesh.meshData.indices.size(),
					GL_UNSIGNED_INT,
					0,
					mesh.instanceCount);
			}
		}
	}
}

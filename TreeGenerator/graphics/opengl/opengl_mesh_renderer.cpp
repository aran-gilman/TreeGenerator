#include "opengl_mesh_renderer.h"

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

#include "shader_program.h"

namespace tree_generator::opengl
{
	OpenGLMeshRenderer::OpenGLMeshRenderer(
		ShaderProgram* materialShader,
		ShaderProgram* normalShader) :
		vertexArray_(0),
		vertexBuffer_(0),
		indexBuffer_(0),
		instanceTransformBuffer_(0),

		indexCount_(0),
		instanceCount_(0),

		material_({}),
		materialShader_(materialShader),
		normalShader_(normalShader)
	{
		glGenBuffers(1, &vertexBuffer_);
		glGenBuffers(1, &indexBuffer_);
		glGenBuffers(1, &instanceTransformBuffer_);
		glGenVertexArrays(1, &vertexArray_);
	}

	OpenGLMeshRenderer::~OpenGLMeshRenderer()
	{
		glDeleteBuffers(1, &vertexBuffer_);
		glDeleteBuffers(1, &indexBuffer_);
		glDeleteBuffers(1, &instanceTransformBuffer_);
		glDeleteVertexArrays(1, &vertexArray_);
	}

	void OpenGLMeshRenderer::SetMeshData(const MeshData& meshData)
	{
		SetMeshData(meshData, { glm::vec3(0), glm::vec3(0), 1.0f});
	}

	void OpenGLMeshRenderer::SetMeshData(
		const MeshData& meshData, const Transform& instance)
	{
		SetMeshData(meshData, std::vector<Transform>({ instance }));
	}

	void OpenGLMeshRenderer::SetMeshData(
		const MeshData& meshData, const std::vector<Transform>& instances)
	{
		indexCount_ = meshData.indices.size();
		instanceCount_ = instances.size();

		glBindVertexArray(vertexArray_);

		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer_);
		glBufferData(
			GL_ARRAY_BUFFER,
			sizeof(Vertex) * meshData.vertices.size(),
			meshData.vertices.data(),
			GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer_);
		glBufferData(
			GL_ELEMENT_ARRAY_BUFFER,
			sizeof(unsigned int) * meshData.indices.size(),
			meshData.indices.data(),
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
		// Sending the entire model matrix for each instance over VBO is
		// a bit expensive (4 vec4s, each taking up a binding point).
		// The main alternative would be to calculate the model matrix from
		// the transform within the shader itself, but those calculations
		// are moderately expensive and repeating them every frame is
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

		glBindBuffer(GL_ARRAY_BUFFER, instanceTransformBuffer_);
		glBufferData(
			GL_ARRAY_BUFFER,
			sizeof(glm::mat4) * instanceCount_,
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

	void OpenGLMeshRenderer::SetMaterial(Material material)
	{
		material_ = material;
	}

	void OpenGLMeshRenderer::Render()
	{
		materialShader_->Bind();
		materialShader_->SetUniform(
			"material.color", material_.color);
		glBindVertexArray(vertexArray_);
		glDrawElementsInstanced(
			GL_TRIANGLES,
			indexCount_,
			GL_UNSIGNED_INT,
			0,
			instanceCount_);
	}
}
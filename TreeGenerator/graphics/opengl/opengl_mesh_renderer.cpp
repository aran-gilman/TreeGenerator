#include "opengl_mesh_renderer.h"

#include <glad/glad.h>

namespace tree_generator::opengl
{
	OpenGLMeshRenderer::OpenGLMeshRenderer() :
		vertexArray_(0),
		vertexBuffer_(0),
		indexBuffer_(0),
		instanceTransformBuffer_(0),

		indexCount_(0),
		instanceCount_(0)
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

	void OpenGLMeshRenderer::Render()
	{

	}
}
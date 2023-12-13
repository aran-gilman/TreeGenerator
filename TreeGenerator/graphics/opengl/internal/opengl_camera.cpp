#include "opengl_camera.h"

#include <glm/gtc/matrix_transform.hpp>

#include "gl_helper.h"

namespace tree_generator::opengl
{
	// This struct primarily serves as a reference for accessing the fields in
	// the camera's uniform block.
	// If making changes to it, make sure it still matches the layout expected
	// by OpenGL and the shader! (e.g. all fields are aligned to a 16-byte
	// boundary)
	struct CameraData
	{
		glm::mat4 view;
		glm::mat4 projection;
	};
	static_assert(
		sizeof(CameraData) % 16 == 0,
		"CameraData must match corresponding uniform block");

	OpenGLCamera::OpenGLCamera(unsigned int uniformBlockIndex) :
		clearColor_(0.0f, 0.0f, 0.0f),
		uniformBuffer_(0),
		viewport_({})
	{
		glGenBuffers(1, &uniformBuffer_);
		glBindBuffer(GL_UNIFORM_BUFFER, uniformBuffer_);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(CameraData), nullptr, GL_STATIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, uniformBlockIndex, uniformBuffer_);
	}

	OpenGLCamera::~OpenGLCamera()
	{
		glDeleteBuffers(1, &uniformBuffer_);
	}

	void OpenGLCamera::SetClearColor(glm::vec3 color)
	{
		clearColor_ = color;
	}

	void OpenGLCamera::SetView(glm::mat4 view)
	{
		glBindBuffer(GL_UNIFORM_BUFFER, uniformBuffer_);
		glBufferSubData(
			GL_UNIFORM_BUFFER,
			offsetof(CameraData, view),
			sizeof(glm::mat4),
			&view);
	}

	void OpenGLCamera::SetViewport(Viewport viewport)
	{
		viewport_ = viewport;
		glm::mat4 projection = glm::perspective(
			glm::radians(45.0f),
			(float)viewport.width / viewport.height,
			0.1f, 1000.0f);
		glBindBuffer(GL_UNIFORM_BUFFER, uniformBuffer_);
		glBufferSubData(
			GL_UNIFORM_BUFFER,
			offsetof(CameraData, projection),
			sizeof(glm::mat4),
			&projection);
	}

	void OpenGLCamera::Bind()
	{
		glViewport(viewport_.x, viewport_.y, viewport_.width, viewport_.height);
		glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
}

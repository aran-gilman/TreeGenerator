#ifndef TREE_GENERATOR_OPENGL_CAMERA_H_
#define TREE_GENERATOR_OPENGL_CAMERA_H_

#include <glm/glm.hpp>

#include "../../common/camera.h"

namespace tree_generator::opengl
{
	class OpenGLCamera : public Camera
	{
	public:
		OpenGLCamera(unsigned int uniformBlockIndex);
		~OpenGLCamera();

		void SetClearColor(glm::vec3 color) override;
		void SetView(glm::mat4 view) override;
		void SetViewport(Viewport viewport) override;

		void Bind() override;

	private:
		Viewport viewport_;
		glm::vec3 clearColor_;
		unsigned int uniformBuffer_;
	};
}

#endif // !TREE_GENERATOR_OPENGL_CAMERA_H_
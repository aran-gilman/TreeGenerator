#ifndef TREE_GENERATOR_CAMERA_H_
#define TREE_GENERATOR_CAMERA_H_

#include <glm/glm.hpp>

namespace tree_generator
{
	class Camera
	{
	public:
		virtual ~Camera() = default;

		virtual void SetClearColor(glm::vec3 color) = 0;
		virtual void SetView(glm::mat4 view) = 0;

		struct Viewport
		{
			int x;
			int y;
			int width;
			int height;
		};
		virtual void SetViewport(Viewport viewport) = 0;

		virtual void Bind() = 0;

	protected:
		Camera() {}
	};
}

#endif // !TREE_GENERATOR_CAMERA_H_
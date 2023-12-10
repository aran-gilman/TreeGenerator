#ifndef TREE_GENERATOR_CAMERA_CONTROLLER_H_
#define TREE_GENERATOR_CAMERA_CONTROLLER_H_

namespace tree_generator
{
	class Renderer;

	class CameraController
	{
	public:
		CameraController(Renderer* renderer);

	private:
		Renderer* renderer_;
	};
}

#endif  // !TREE_GENERATOR_CAMERA_CONTROLLER_H_
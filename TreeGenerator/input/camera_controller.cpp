#include "camera_controller.h"

#include "../graphics/common/renderer.h"

namespace tree_generator
{
	CameraController::CameraController(Renderer* renderer) :
		CameraController(renderer, MovementSettings())
	{
	}
	
	CameraController::CameraController(
		Renderer* renderer, MovementSettings movementSettings) :
		renderer_(renderer),
		movementSettings_(movementSettings)
	{

	}

	void CameraController::Update(double elapsedTime)
	{

	}
}

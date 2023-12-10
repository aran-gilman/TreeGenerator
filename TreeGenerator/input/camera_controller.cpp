#include "camera_controller.h"

#include <glm/glm.hpp>
#include <glm/gtx/common.hpp>

#include "../graphics/common/renderer.h"

namespace tree_generator
{
	namespace
	{
		// Setting the horizontal angle range to strictly positive makes the
		// math a bit more elegant when the camera loops around.
		constexpr float kMaxHorizontalAngle = 360.0f;

		// We stop just below 90 degrees to keep the camera from flipping.
		constexpr float kMaxVerticalAngle = 89.0f;

		// Only update the camera position if it has at least this much
		// movement along any axis. This ensures we only send data to the GPU
		// when there has been a change.
		constexpr float kMinUpdateSpeed = 0.001f;
	}

	CameraController::CameraController(Renderer* renderer) :
		CameraController(renderer, MovementSettings())
	{
	}
	
	CameraController::CameraController(
		Renderer* renderer, MovementSettings movementSettings) :
		renderer_(renderer),
		movementSettings_(movementSettings)
	{
		UpdateCameraPosition();
	}

	void CameraController::Update(double elapsedTime)
	{
		bool wasPositionUpdated = false;
		if (glm::abs(currentMovement_.horizontalRotationVelocity) > kMinUpdateSpeed)
		{
			currentPosition_.horizontalAngle += (
				currentMovement_.horizontalRotationVelocity * elapsedTime);
			currentPosition_.horizontalAngle = glm::fmod(
				currentPosition_.horizontalAngle, kMaxHorizontalAngle);
			if (currentPosition_.horizontalAngle < -kMaxHorizontalAngle)
			{
				currentPosition_.horizontalAngle += kMaxHorizontalAngle;
			}
			wasPositionUpdated = true;
		}

		if (glm::abs(currentMovement_.verticalRotationVelocity) > kMinUpdateSpeed)
		{
			currentPosition_.verticalAngle += (
				currentMovement_.verticalRotationVelocity * elapsedTime);
			if (currentPosition_.verticalAngle > kMaxVerticalAngle)
			{
				currentPosition_.verticalAngle = kMaxVerticalAngle;
			}
			else if (currentPosition_.verticalAngle < -kMaxVerticalAngle)
			{
				currentPosition_.verticalAngle = -kMaxVerticalAngle;
			}
			wasPositionUpdated = true;
		}

		if (wasPositionUpdated)
		{
			UpdateCameraPosition();
		}
	}

	void CameraController::UpdateCameraPosition()
	{
		float verticalMultiplier = glm::cos(glm::radians(currentPosition_.verticalAngle));

		float x = verticalMultiplier * glm::sin(
			glm::radians(currentPosition_.horizontalAngle));
		float y = glm::sin(glm::radians(currentPosition_.verticalAngle));
		float z = verticalMultiplier * glm::cos(
			glm::radians(currentPosition_.horizontalAngle));

		glm::vec3 newPosition{ x, y, z };

		renderer_->SetCameraPosition(newPosition * currentPosition_.distance);
	}
}

#include "camera_controller.h"

#include <glm/glm.hpp>
#include <glm/gtx/common.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../graphics/common/camera.h"

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

	CameraController::CameraController(Camera* camera) :
		CameraController(camera, MovementSettings())
	{
	}

	CameraController::CameraController(
		Camera* camera, MovementSettings movementSettings) :
		camera_(camera),
		movementSettings_(movementSettings)
	{
		UpdateCamera();
	}

	void CameraController::Update(double elapsedTime)
	{
		bool wasPositionUpdated = false;
		wasPositionUpdated |= UpdateHorizontalPosition(elapsedTime);
		wasPositionUpdated |= UpdateVerticalPosition(elapsedTime);
		wasPositionUpdated |= UpdateDistance(elapsedTime);
		wasPositionUpdated |= UpdateHeight(elapsedTime);

		if (wasPositionUpdated)
		{
			UpdateCamera();
		}
	}

	void CameraController::UpdateCamera()
	{
		float verticalMultiplier = glm::cos(glm::radians(currentPosition_.verticalAngle));

		float x = verticalMultiplier * glm::sin(
			glm::radians(currentPosition_.horizontalAngle));
		float y = glm::sin(glm::radians(currentPosition_.verticalAngle));
		float z = verticalMultiplier * glm::cos(
			glm::radians(currentPosition_.horizontalAngle));

		glm::vec3 newPosition = glm::vec3(x, y, z) * currentPosition_.distance;
		newPosition += glm::vec3(0.0f, currentPosition_.height, 0.0f);

		glm::mat4 view = glm::lookAt(
			newPosition,
			glm::vec3(0.0f, currentPosition_.height, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f));
		camera_->SetView(view);
	}

	bool CameraController::UpdateHorizontalPosition(double elapsedTime)
	{
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
			return true;
		}
		return false;
	}

	bool CameraController::UpdateVerticalPosition(double elapsedTime)
	{
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
			return true;
		}
		return false;
	}

	bool CameraController::UpdateDistance(double elapsedTime)
	{
		if (currentMovement_.remainingDistanceChange > kMinUpdateSpeed)
		{
			float diff = movementSettings_.maxDistanceChangePerSecond * elapsedTime;
			float newRemainingDistance = currentMovement_.remainingDistanceChange - diff;
			if (newRemainingDistance - diff < 0.0f)
			{
				diff = currentMovement_.remainingDistanceChange;
				currentMovement_.remainingDistanceChange = 0;
			}
			else
			{
				currentMovement_.remainingDistanceChange = newRemainingDistance;
			}
			currentPosition_.distance += diff;
			if (currentPosition_.distance > movementSettings_.maxDistance)
			{
				currentPosition_.distance = movementSettings_.maxDistance;
			}
			return true;
		}
		else if (currentMovement_.remainingDistanceChange < -kMinUpdateSpeed)
		{
			float diff = -movementSettings_.maxDistanceChangePerSecond * elapsedTime;
			float newRemainingDistance = currentMovement_.remainingDistanceChange - diff;
			if (newRemainingDistance - diff > 0.0f)
			{
				diff = currentMovement_.remainingDistanceChange;
				currentMovement_.remainingDistanceChange = 0;
			}
			else
			{
				currentMovement_.remainingDistanceChange = newRemainingDistance;
			}
			currentPosition_.distance += diff;
			if (currentPosition_.distance < movementSettings_.minDistance)
			{
				currentPosition_.distance = movementSettings_.minDistance;
			}
			return true;
		}
		return false;
	}

	bool CameraController::UpdateHeight(double elapsedTime)
	{
		if (glm::abs(currentMovement_.heightVelocity) > kMinUpdateSpeed)
		{
			currentPosition_.height += (currentMovement_.heightVelocity * elapsedTime);
			if (currentPosition_.height > movementSettings_.maxHeight)
			{
				currentPosition_.height = movementSettings_.maxHeight;
			}
			else if (currentPosition_.height < movementSettings_.minHeight)
			{
				currentPosition_.height = movementSettings_.minHeight;
			}
			return true;
		}
		return false;
	}
}

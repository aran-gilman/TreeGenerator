#ifndef TREE_GENERATOR_CAMERA_CONTROLLER_H_
#define TREE_GENERATOR_CAMERA_CONTROLLER_H_

namespace tree_generator
{
	class Renderer;

	class CameraController
	{
	public:
		struct MovementSettings
		{
			float maxHorizontalRotationPerSecond = 180.0f;
			float maxVerticalRotationPerSecond = 180.0f;

			float minDistance = 1.0f;
			float maxDistance = 10.0f;
			float maxDistanceChangePerSecond = 5.0f;
		};

		struct Movement
		{
			float horizontalRotationVelocity = 0.0f;
			float verticalRotationVelocity = 0.0f;
			float remainingDistanceChange = 0.0f;
		};

		struct Position
		{
			float horizontalAngle = 0.0f;
			float verticalAngle = 0.0f;
			float distance = 0.0f;
		};

		CameraController(Renderer* renderer);
		CameraController(Renderer* renderer, MovementSettings movementSettings);

		void Update(double elapsedTime);

		MovementSettings& GetMovementSettings() { return movementSettings_; }
		Movement& GetCurrentMovement() { return currentMovement_; }
		Position& GetCurrentPosition() { return currentPosition_; }

	private:
		Renderer* renderer_;

		MovementSettings movementSettings_;
		Movement currentMovement_;
		Position currentPosition_;
	};
}

#endif  // !TREE_GENERATOR_CAMERA_CONTROLLER_H_
#ifndef TREE_GENERATOR_CAMERA_CONTROLLER_H_
#define TREE_GENERATOR_CAMERA_CONTROLLER_H_

namespace tree_generator
{
	class Camera;

	// Rotates the renderer's camera around the origin of the scene.
	// 
	// The camera can rotate all the way around on the horizontal plane.
	// Movement along the vertical plane is restricted to less than 90 degrees
	// in either direction to keep the camera from going upside-down.
	// 
	// All angles are in degrees
	class CameraController
	{
	public:
		// Configures how far and how much the camera can move
		struct MovementSettings
		{
			// The camera's movement along the horizontal plane
			float maxHorizontalRotationPerSecond = 180.0f;

			// The camera's movmeent along the vertical plane
			float maxVerticalRotationPerSecond = 180.0f;

			float minDistance = 1.0f;
			float maxDistance = 100.0f;
			float maxDistanceChangePerSecond = 10.0f;

			float minHeight = 0.0f;
			float maxHeight = 100.0f;
			float maxHeightChangePerSecond = 10.0f;
		};

		struct Movement
		{
			float horizontalRotationVelocity = 0.0f;
			float verticalRotationVelocity = 0.0f;
			float heightVelocity = 0.0f;
			float remainingDistanceChange = 0.0f;
		};

		struct Position
		{
			// Range: 0 to 360
			float horizontalAngle = 0.0f;

			// Range: -90 to 90
			float verticalAngle = 0.0f;

			float distance = 50.0f;
			float height = 1.0f;
		};

		CameraController(Camera* camera);
		CameraController(Camera* camera, MovementSettings movementSettings);

		void Update(double elapsedTime);

		MovementSettings& GetMovementSettings() { return movementSettings_; }
		Movement& GetCurrentMovement() { return currentMovement_; }
		Position& GetCurrentPosition() { return currentPosition_; }

	private:
		Camera* camera_;

		MovementSettings movementSettings_;
		Movement currentMovement_;
		Position currentPosition_;

		void UpdateCamera();

		// Returns whether the associated value actually changed.
		bool UpdateHorizontalPosition(double elapsedTime);
		bool UpdateVerticalPosition(double elapsedTime);
		bool UpdateDistance(double elapsedTime);
		bool UpdateHeight(double elapsedTime);
	};
}

#endif  // !TREE_GENERATOR_CAMERA_CONTROLLER_H_
#ifndef TREE_GENERATOR_APP_H_
#define TREE_GENERATOR_APP_H_

#include <memory>

#include "lsystem/rendering/mesh_generator.h"
#include "lsystem/rendering/string_generator.h"

namespace tree_generator
{
	class CameraController;
	class Renderer;
	class Window;

	// TODO: Replace this with runtime-/user-configurable structure
	struct DisplaySymbols
	{
		lsystem::Symbol trunk{ '1' };
		lsystem::Symbol leaf{ '0' };

		lsystem::Symbol push{ '[' };
		lsystem::Symbol pop{ ']' };

		lsystem::Symbol rotateRight{ 'R' };
		lsystem::Symbol rotateLeft{ 'L' };
		lsystem::Symbol advance{ 'A' };
	};

	class TreeGeneratorApp
	{
	public:
		TreeGeneratorApp();
		~TreeGeneratorApp();

		void Run();

	private:
		std::unique_ptr<Window> window_;
		std::unique_ptr<Renderer> renderer_;
		std::unique_ptr<CameraController> cameraController_;

		DisplaySymbols symbols_;
		lsystem::MeshGenerator meshGenerator_;
		lsystem::StringGenerator stringGenerator_;
	};
}

#endif  // !TREE_GENERATOR_APP_H_
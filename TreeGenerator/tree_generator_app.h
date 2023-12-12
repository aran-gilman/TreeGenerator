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

	class TreeGeneratorApp
	{
	public:
		void Run();

	private:
		std::unique_ptr<Window> window_;
		std::unique_ptr<Renderer> renderer_;
		std::unique_ptr<CameraController> cameraController_;

		lsystem::MeshGenerator meshGenerator_;
		lsystem::StringGenerator stringGenerator_;
	};
}

#endif  // !TREE_GENERATOR_APP_H_
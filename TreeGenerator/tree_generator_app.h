#ifndef TREE_GENERATOR_APP_H_
#define TREE_GENERATOR_APP_H_

#include <memory>
#include <vector>

#include "lsystem/core/lsystem.h"
#include "lsystem/core/lsystem_parser.h"
#include "lsystem/rendering/mesh_generator.h"

namespace tree_generator
{
	class Camera;
	class CameraController;
	class MeshRenderer;
	class RenderContext;
	class Window;

	class TreeGeneratorApp
	{
	public:
		TreeGeneratorApp();
		~TreeGeneratorApp();

		void Run();

	private:
		std::unique_ptr<Window> window_;
		std::unique_ptr<RenderContext> renderer_;
		std::unique_ptr<Camera> camera_;
		std::unique_ptr<CameraController> cameraController_;

		lsystem::StringLSystem stringLSystem_;
		lsystem::MeshGenerator meshGenerator_;

		std::vector<std::unique_ptr<MeshRenderer>> meshes_;

		bool showDemoWindow_;
		int iterations_;
		bool doOutputToConsole_;
		bool doShowNormals_;
		std::string newSymbolInput_;

		void ShowMenu();

		void ShowGenerateButton();
		void ShowLSystemSection();
		void ShowMeshSection();
		void ShowDebugSection();
	};
}

#endif  // !TREE_GENERATOR_APP_H_
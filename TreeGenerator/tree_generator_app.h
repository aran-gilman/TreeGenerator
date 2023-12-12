#ifndef TREE_GENERATOR_APP_H_
#define TREE_GENERATOR_APP_H_

#include <memory>

#include "lsystem/core/lsystem.h"
#include "lsystem/core/lsystem_parser.h"
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
		lsystem::Symbol trunk{ 'F' };
		lsystem::Symbol leaf{ 'X' };

		lsystem::Symbol push{ '[' };
		lsystem::Symbol pop{ ']' };

		lsystem::Symbol rotateRight{ '-' };
		lsystem::Symbol rotateLeft{ '+' };
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
		lsystem::StringLSystem stringLSystem_;
		lsystem::LSystem lSystem_;

		lsystem::MeshGenerator meshGenerator_;
		lsystem::StringGenerator stringGenerator_;

		bool showDemoWindow_;
		int iterations_;
		bool doOutputToConsole_;

		void ShowMenu();

		void ShowGenerateButton();
		void ShowLSystemSection();
		void ShowMeshSection();
		void ShowDebugSection();
	};
}

#endif  // !TREE_GENERATOR_APP_H_
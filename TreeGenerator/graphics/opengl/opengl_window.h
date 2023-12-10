#ifndef TREE_GENERATOR_OPENGL_WINDOW_H_
#define TREE_GENERATOR_OPENGL_WINDOW_H_

#include <tuple>

#include "../common/window.h"

struct GLFWwindow;

namespace tree_generator
{
	namespace opengl
	{
		class OpenGLWindow : public Window
		{
		public:
			OpenGLWindow(int width, int height, const std::string& title);
			~OpenGLWindow() override;

			int Width() const override { return width_; }
			int Height() const override { return height_; }

			void Display(RenderCallback renderCallback) override;

		private:
			int width_;
			int height_;

			GLFWwindow* internalWindow_;
		};
	}
}

#endif  // !TREE_GENERATOR_OPENGL_WINDOW_H_
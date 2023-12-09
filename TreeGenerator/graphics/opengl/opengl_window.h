#ifndef TREE_GENERATOR_OPENGL_WINDOW_H_
#define TREE_GENERATOR_OPENGL_WINDOW_H_

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

			void Display(RenderCallback renderCallback) override;

		private:
			GLFWwindow* internalWindow_;
		};
	}
}

#endif  // !TREE_GENERATOR_OPENGL_WINDOW_H_
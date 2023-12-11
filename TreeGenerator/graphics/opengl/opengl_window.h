#ifndef TREE_GENERATOR_OPENGL_WINDOW_H_
#define TREE_GENERATOR_OPENGL_WINDOW_H_

#include <functional>
#include <tuple>

#include "../common/key_action.h"
#include "../common/key_token.h"
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
			void SetKeyboardCallback(KeyboardCallback keyboardCallback) override; 
			void SetScrollCallback(ScrollCallback scrollCallback) override;

		private:
			static void ReceiveKeyboardEvent(
				GLFWwindow* window, int keyToken, int scancode, int action, int mods);
			static void ReceiveScrollEvent(
				GLFWwindow* window, double xOffset, double yOffset);

			int width_;
			int height_;

			GLFWwindow* internalWindow_;
			KeyboardCallback keyboardCallback_;
			ScrollCallback scrollCallback_;

			void SendKeyboardEvent(KeyToken token, KeyAction action);
			void SendScrollEvent(double xOffset, double yOffset);
		};
	}
}

#endif  // !TREE_GENERATOR_OPENGL_WINDOW_H_
#include "opengl_window.h"

#include <stdexcept>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace tree_generator
{
	namespace opengl
	{
		namespace
		{
			void HandleKeyboardInput(
				GLFWwindow* window,
				int keyToken, int scancode, int action, int mods)
			{
				OpenGLWindow* user = static_cast<OpenGLWindow*>(
					glfwGetWindowUserPointer(window));
				user->SendKeyboardEvent(
					static_cast<KeyToken>(keyToken),
					static_cast<KeyAction>(action));
			}
		}

		OpenGLWindow::OpenGLWindow(int width, int height, const std::string& title) :
			width_(width),
			height_(height),
			internalWindow_(nullptr),
			keyboardCallback_([](KeyToken, KeyAction) {})
		{
			glfwInit();
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

			internalWindow_ = glfwCreateWindow(
				width, height, title.c_str(), nullptr, nullptr);
			if (internalWindow_ == nullptr)
			{
				glfwTerminate();
				throw std::runtime_error("Failed to create GLFW window");
			}

			glfwMakeContextCurrent(internalWindow_);
			glfwSetWindowUserPointer(internalWindow_, this);

			glfwSetKeyCallback(internalWindow_, HandleKeyboardInput);
		}

		OpenGLWindow::~OpenGLWindow()
		{
			glfwDestroyWindow(internalWindow_);
			glfwTerminate();
		}

		void OpenGLWindow::Display(RenderCallback renderCallback)
		{
			double currentTime = 0;
			double previousTime = glfwGetTime();
			while (!glfwWindowShouldClose(internalWindow_))
			{
				currentTime = glfwGetTime();
				renderCallback(currentTime - previousTime);
				glfwSwapBuffers(internalWindow_);
				glfwPollEvents();
				previousTime = currentTime;
			}
		}
		
		void OpenGLWindow::SetKeyboardCallback(KeyboardCallback keyboardCallback)
		{
			keyboardCallback_ = keyboardCallback;
		}

		void OpenGLWindow::SendKeyboardEvent(KeyToken token, KeyAction action)
		{
			keyboardCallback_(token, action);
		}
	}
}
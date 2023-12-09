#include "opengl_window.h"

#include <stdexcept>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace tree_generator
{
	namespace opengl
	{
		OpenGLWindow::OpenGLWindow(int width, int height, const std::string& title) :
			internalWindow_(nullptr)
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

			int version = gladLoadGL();
			if (version == 0)
			{
				glfwDestroyWindow(internalWindow_);
				glfwTerminate();
				throw std::runtime_error("Failed to load OpenGL");
			}

			glViewport(0, 0, width, height);
		}

		OpenGLWindow::~OpenGLWindow()
		{
			glfwDestroyWindow(internalWindow_);
			glfwTerminate();
		}

		void OpenGLWindow::Display(RenderCallback renderCallback)
		{
			while (!glfwWindowShouldClose(internalWindow_))
			{
				renderCallback();
				glfwSwapBuffers(internalWindow_);
				glfwPollEvents();
			}
		}
	}
}
#include "Window.h"

#include <stdexcept>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace tree_generator
{
	namespace
	{
		// TODO: Separate Window and graphics library into their own classes
		class WindowGlfwGlad : public Window
		{
		public:
			WindowGlfwGlad(int width, int height, const std::string& title);
			~WindowGlfwGlad() override;

			void Display() override;

		private:
			GLFWwindow* internalWindow_;
		};

		WindowGlfwGlad::WindowGlfwGlad(int width, int height, const std::string& title) :
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

		WindowGlfwGlad::~WindowGlfwGlad()
		{
			glfwDestroyWindow(internalWindow_);
			glfwTerminate();
		}

		void WindowGlfwGlad::Display()
		{
			while (!glfwWindowShouldClose(internalWindow_))
			{
				glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
				glClear(GL_COLOR_BUFFER_BIT);
				glfwSwapBuffers(internalWindow_);
				glfwPollEvents();
			}
		}
	}  // namespace

	std::unique_ptr<Window> Window::Create(int width, int height, const std::string& title)
	{
		return std::make_unique<WindowGlfwGlad>(width, height, title);
	}
}  // namespace tree_generator

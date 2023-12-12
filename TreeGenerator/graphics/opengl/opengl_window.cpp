#include "opengl_window.h"

#include <stdexcept>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

namespace tree_generator
{
	namespace opengl
	{
		OpenGLWindow::OpenGLWindow(int width, int height, const std::string& title) :
			width_(width),
			height_(height),
			internalWindow_(nullptr),
			keyboardCallback_([](KeyToken, KeyAction) {}),
			scrollCallback_([](double, double) {})
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

			glfwSetKeyCallback(internalWindow_, ReceiveKeyboardEvent);
			glfwSetScrollCallback(internalWindow_, ReceiveScrollEvent);

			IMGUI_CHECKVERSION();
			ImGui::CreateContext();

			ImGuiIO& io = ImGui::GetIO();
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
			ImGui::StyleColorsDark();

			ImGui_ImplGlfw_InitForOpenGL(internalWindow_, true);
			ImGui_ImplOpenGL3_Init(nullptr);
		}

		OpenGLWindow::~OpenGLWindow()
		{
			ImGui_ImplOpenGL3_Shutdown();
			ImGui_ImplGlfw_Shutdown();
			ImGui::DestroyContext();

			glfwDestroyWindow(internalWindow_);
			glfwTerminate();
		}

		void OpenGLWindow::Display(RenderCallback renderCallback)
		{
			double currentTime = 0;
			double previousTime = glfwGetTime();
			while (!glfwWindowShouldClose(internalWindow_))
			{
				ImGui_ImplOpenGL3_NewFrame();
				ImGui_ImplGlfw_NewFrame();
				ImGui::NewFrame();

				currentTime = glfwGetTime();
				renderCallback(currentTime - previousTime);

				ImGui::Render();
				ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

				glfwSwapBuffers(internalWindow_);
				glfwPollEvents();
				previousTime = currentTime;
			}
		}

		void OpenGLWindow::SetKeyboardCallback(KeyboardCallback keyboardCallback)
		{
			keyboardCallback_ = keyboardCallback;
		}

		void OpenGLWindow::SetScrollCallback(ScrollCallback scrollCallback)
		{
			scrollCallback_ = scrollCallback;
		}

		void OpenGLWindow::ReceiveKeyboardEvent(
			GLFWwindow* window,
			int keyToken, int scancode, int action, int mods)
		{
			OpenGLWindow* user = static_cast<OpenGLWindow*>(
				glfwGetWindowUserPointer(window));
			user->SendKeyboardEvent(
				static_cast<KeyToken>(keyToken),
				static_cast<KeyAction>(action));
		}

		void OpenGLWindow::ReceiveScrollEvent(
			GLFWwindow* window, double xOffset, double yOffset)
		{
			OpenGLWindow* user = static_cast<OpenGLWindow*>(
				glfwGetWindowUserPointer(window));
			user->SendScrollEvent(xOffset, yOffset);
		}

		void OpenGLWindow::SendKeyboardEvent(KeyToken token, KeyAction action)
		{
			keyboardCallback_(token, action);
		}

		void OpenGLWindow::SendScrollEvent(double xOffset, double yOffset)
		{
			scrollCallback_(xOffset, yOffset);
		}
	}
}
#ifndef TREE_GENERATOR_WINDOW_H_
#define TREE_GENERATOR_WINDOW_H_

#include <memory>
#include <string>

namespace tree_generator
{
	class Window
	{
	public:
		static std::unique_ptr<Window> Create(int width, int height, const std::string& title);

		virtual ~Window() {}

		virtual void Display() = 0;

		// Disallow copy and move
		Window(const Window&) = delete;
		Window& operator&(const Window&) = delete;
		Window(Window&&) = delete;
		Window& operator&(Window&&) = delete;

	protected:
		Window() {}
	};
}

#endif  // !TREE_GENERATOR_WINDOW_H_
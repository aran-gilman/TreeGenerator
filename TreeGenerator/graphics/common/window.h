#ifndef TREE_GENERATOR_WINDOW_H_
#define TREE_GENERATOR_WINDOW_H_

#include <functional>
#include <string>

namespace tree_generator
{
	class Window
	{
	public:
		using RenderCallback = std::function<void()>;

		virtual ~Window() {}

		virtual void Display(RenderCallback renderCallback) = 0;

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
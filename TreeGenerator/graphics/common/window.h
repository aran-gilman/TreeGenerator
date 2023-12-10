#ifndef TREE_GENERATOR_WINDOW_H_
#define TREE_GENERATOR_WINDOW_H_

#include <functional>
#include <string>

#include "key_action.h"
#include "key_token.h"

namespace tree_generator
{
	class Window
	{
	public:
		virtual ~Window() {}

		virtual int Width() const = 0;
		virtual int Height() const = 0;

		using RenderCallback = std::function<void(double elapsedTime)>;
		virtual void Display(RenderCallback renderCallback) = 0;

		using KeyboardCallback = std::function<void(KeyToken, KeyAction)>;
		virtual void SetKeyboardCallback(KeyboardCallback keyboardCallback) = 0;

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
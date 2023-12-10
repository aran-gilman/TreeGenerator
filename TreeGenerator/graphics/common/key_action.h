#ifndef TREE_GENERATOR_KEY_ACTION_H_
#define TREE_GENERATOR_KEY_ACTION_H_

namespace tree_generator
{
	// These values are the same as the glfw enum-like macros with matching name.
	// This allows us to static_cast to and from the glfw values.
	// We don't use the macros directly to avoid including glfw and generating a
	// hard dependency on it.
	enum class KeyAction : int
	{
		Press = 1,
		Release = 0,
		Repeat = 2,
	};
}

#endif // !TREE_GENERATOR_KEY_ACTION_H_
#ifndef TREE_GENERATOR_TRANSFORM_H_
#define TREE_GENERATOR_TRANSFORM_H_

#include <glm/glm.hpp>

namespace tree_generator
{
	struct Transform
	{
		glm::vec3 position;
		glm::vec3 rotation;
		float scale;
	};
}

#endif  // !TREE_GENERATOR_TRANSFORM_H_
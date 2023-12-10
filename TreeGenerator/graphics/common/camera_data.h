#ifndef TREE_GENERATOR_CAMERA_DATA_H_
#define TREE_GENERATOR_CAMERA_DATA_H_

#include <glm/glm.hpp>

namespace tree_generator
{
	struct CameraData
	{
		glm::mat4 view;
		glm::mat4 projection;
	};
}

#endif //  !TREE_GENERATOR_CAMERA_DATA_H_
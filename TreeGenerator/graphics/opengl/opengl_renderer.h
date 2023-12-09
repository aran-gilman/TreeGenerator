#ifndef TREE_GENERATOR_OPENGL_RENDERER_H_
#define TREE_GENERATOR_OPENGL_RENDERER_H_

#include "../renderer.h"

namespace tree_generator
{
	class OpenGLRenderer : public Renderer
	{
	public:
		~OpenGLRenderer() override;

		void AddMesh() override;
		void Render() override;
	};
}

#endif  // !TREE_GENERATOR_OPENGL_RENDERER_H_
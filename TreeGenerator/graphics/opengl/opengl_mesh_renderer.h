#ifndef TREE_GENERATOR_OPENGL_MESH_RENDERER_H_
#define TREE_GENERATOR_OPENGL_MESH_RENDERER_H_

#include "../common/mesh_renderer.h"

namespace tree_generator::opengl
{
	class OpenGLMeshRenderer : public MeshRenderer
	{
	public:
		void Render() override;
	};
}

#endif // !TREE_GENERATOR_OPENGL_MESH_RENDERER_H_
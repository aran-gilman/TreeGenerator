#ifndef TREE_GENERATOR_MESH_RENDERER_H_
#define TREE_GENERATOR_MESH_RENDERER_H_

namespace tree_generator
{
	class MeshRenderer
	{
	public:
		virtual void Render() = 0;

	protected:
		MeshRenderer() {}
	};
}

#endif // !TREE_GENERATOR_MESH_RENDERER_H_
#include "mesh_generator.h"

namespace tree_generator::lsystem
{
	void MeshGenerator::DefineDraw(const Symbol& symbol, MeshData meshData)
	{

	}

	void MeshGenerator::DefineMove(const Symbol& symbol)
	{

	}

	void MeshGenerator::DefineRotate(const Symbol& symbol, glm::vec3 rotation)
	{

	}

	void MeshGenerator::DefineSave(const Symbol& symbol)
	{

	}

	void MeshGenerator::DefineRestore(const Symbol& symbol)
	{

	}

	std::vector<MeshGenerator::MeshGroup> MeshGenerator::Generate(
		const std::vector<Symbol>& symbols) const
	{
		return std::vector<MeshGroup>();
	}
}

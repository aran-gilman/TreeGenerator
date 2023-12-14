#ifndef TREE_GENERATOR_LSYSTEM_MESH_DEFINITION_H_
#define TREE_GENERATOR_LSYSTEM_MESH_DEFINITION_H_

#include <string>
#include <string_view>

#include "../../graphics/common/mesh_data.h"

namespace tree_generator::lsystem
{
	class MeshDefinition
	{
	public:
		virtual ~MeshDefinition() = default;

		virtual bool ShowGUI() = 0;
		virtual const std::string_view Name() const = 0;
		virtual MeshData GenerateMesh() const = 0;
	};

	class CylinderDefinition : public MeshDefinition
	{
	public:
		CylinderDefinition(int sideCount, float height, float radius);

		bool ShowGUI() override;
		const std::string_view Name() const override { return kName_; }
		MeshData GenerateMesh() const override;

	private:
		inline static const std::string kName_ = "Cylinder";
		int sideCount_;
		float height_;
		float radius_;
	};


	class QuadDefinition : public MeshDefinition
	{
	public:
		QuadDefinition();

		bool ShowGUI() override;
		const std::string_view Name() const override { return kName_; }
		MeshData GenerateMesh() const override;

	private:
		inline static const std::string kName_ = "Quad";
		float width_;
		float height_;
		float skew_;
	};
}

#endif // !TREE_GENERATOR_LSYSTEM_MESH_DEFINITION_H_
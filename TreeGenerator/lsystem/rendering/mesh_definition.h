#ifndef TREE_GENERATOR_LSYSTEM_MESH_DEFINITION_H_
#define TREE_GENERATOR_LSYSTEM_MESH_DEFINITION_H_

#include <memory>
#include <string>
#include <string_view>

#include "../../graphics/common/mesh_data.h"
#include "../../utility/enum_helper.h"

namespace tree_generator::lsystem
{
	enum class MeshType : int
	{
		Cylinder,
		Quad,
	};
	using MeshTypeIterator = utility::EnumIteratorInclusive<
		MeshType,
		MeshType::Cylinder,
		MeshType::Quad>;

	std::string GetName(MeshType meshType);

	class MeshDefinition
	{
	public:
		static std::unique_ptr<MeshDefinition> FromMeshType(MeshType meshType);

		virtual ~MeshDefinition() = default;

		virtual bool ShowGUI() = 0;
		virtual const std::string_view Name() const = 0;
		virtual MeshData GenerateMesh() const = 0;

		virtual MeshType GetMeshType() const = 0;
	};

	class CylinderDefinition : public MeshDefinition
	{
	public:
		CylinderDefinition(int sideCount, float height, float radius);

		bool ShowGUI() override;
		const std::string_view Name() const override { return kName_; }
		MeshData GenerateMesh() const override;

		MeshType GetMeshType() const override { return MeshType::Cylinder; }

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

		MeshType GetMeshType() const override { return MeshType::Quad; }

	private:
		inline static const std::string kName_ = "Quad";
		float width_;
		float height_;
		float skew_;
	};
}

#endif // !TREE_GENERATOR_LSYSTEM_MESH_DEFINITION_H_
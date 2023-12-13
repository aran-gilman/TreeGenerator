#include "mesh_generator.h"

#include <ostream>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <glm/glm.hpp>

#include "../core/lsystem.h"
#include "../../graphics/common/mesh_data.h"
#include "../../graphics/common/transform.h"

using ::testing::AllOf;
using ::testing::ElementsAre;
using ::testing::ElementsAreArray;
using ::testing::Eq;
using ::testing::Field;
using ::testing::FieldsAre;
using ::testing::IsEmpty;
using ::testing::PrintToString;
using ::testing::SizeIs;

namespace glm
{
	// These functions are not in an anonymous namespace because GoogleTest
	// cannot find them otherwise.
	void PrintTo(const glm::vec3& vec, std::ostream* os)
	{
		*os << "(x: " << vec.x << ", y: " << vec.y << ", z: " << vec.z << ")";
	}

	void PrintTo(const glm::vec2& vec, std::ostream* os)
	{
		*os << "(x: " << vec.x << ", y: " << vec.y << ")";
	}
}

namespace tree_generator
{
	void PrintTo(const Transform& transform, std::ostream* os)
	{
		*os << "\nTransform(\n\tposition: " << PrintToString(transform.position);
		*os << ",\n\trotation: " << PrintToString(transform.rotation);
		*os << ",\n\tscale: " << transform.scale << ")";
	}

	void PrintTo(const Vertex& vertex, std::ostream* os)
	{
		*os << "\nVertex(\n\tposition: " << PrintToString(vertex.position);
		*os << ",\n\tnormal: " << PrintToString(vertex.normal);
		*os << ",\n\tuv: " << PrintToString(vertex.uv) << ")";
	}

	void PrintTo(const MeshData& meshData, std::ostream* os)
	{
		*os << "\nMeshData(\n\tvertices: " << PrintToString(meshData.vertices);
		*os << ",\n\tindices: " << PrintToString(meshData.indices) << ")";
	}
}

namespace tree_generator::lsystem
{
	void PrintTo(const MeshGroup& meshGroup, std::ostream* os)
	{
		*os << "\nMeshGroup(\n\tmesh: " << PrintToString(meshGroup.mesh);
		*os << ",\n\tinstances: " << PrintToString(meshGroup.instances) << ")";
	}

	namespace
	{
		TEST(LSystemStringGeneratorTest, NoSymbolsGeneratesNoMeshes)
		{
			MeshGenerator generator;
			EXPECT_THAT(generator.Generate({}), IsEmpty());
		}

		TEST(LSystemMeshGeneratorTest, NoDefinitionsGeneratesNoMeshes)
		{
			Symbol a{ 'a' };
			MeshGenerator generator;
			EXPECT_THAT(generator.Generate({ a }), IsEmpty());
		}

		TEST(LSystemMeshGeneratorTest, SymbolGeneratesMeshDataMatchingDefinition)
		{
			Symbol a{ 'a' };
			MeshData quad = CreateQuad();

			MeshGenerator generator;
			generator.Define(a,
				std::make_unique<DrawAction>(
					std::make_unique<QuadDefinition>(),
					Material()));

			// MeshData and Vertex don't have equality defined on them, so we
			// can't directly use equals. For now, assume that if the size is
			// the same, the vertices were set correctly.
			EXPECT_THAT(
				generator.Generate({ a }),
				ElementsAre(
					Field(&MeshGroup::mesh,
						AllOf(
							Field(&MeshData::indices, ElementsAreArray(quad.indices)),
							Field(&MeshData::vertices, SizeIs(quad.vertices.size()))
						))));
		}

		TEST(LSystemMeshGeneratorTest, RepeatedSymbolsAddedToSameMeshGroup)
		{
			Symbol a{ 'a' };
			MeshData quad = CreateQuad();

			MeshGenerator generator;
			generator.Define(a, std::make_unique<DrawAction>(
				std::make_unique<QuadDefinition>(),
				Material()));

			EXPECT_THAT(
				generator.Generate({ a, a }),
				ElementsAre(
					Field(&MeshGroup::instances, SizeIs(2))
				));
		}

		TEST(LSystemMeshGeneratorTest, MoveAdvancesTransform)
		{
			Symbol symbolDraw{ 'a' };
			Symbol symbolMove{ 'b' };

			MeshGenerator generator;
			generator.Define(symbolDraw,
				std::make_unique<DrawAction>(
					std::make_unique<QuadDefinition>(), Material()));
			generator.Define(symbolMove, std::make_unique<MoveAction>());

			EXPECT_THAT(
				generator.Generate({ symbolDraw, symbolMove, symbolDraw }),
				ElementsAre(
					Field(&MeshGroup::instances,
						ElementsAre(
							Field(&Transform::position, Eq(glm::vec3(0))),
							Field(&Transform::position, Eq(glm::vec3(0.0f, 1.0f, 0.0f)))
						))));
		}

		TEST(LSystemMeshGeneratorTest, RotateRotatesTransform)
		{
			Symbol symbolDraw{ 'a' };
			Symbol symbolRotate{ 'b' };

			MeshGenerator generator;
			generator.Define(symbolDraw,
				std::make_unique<DrawAction>(
					std::make_unique<QuadDefinition>(),
					Material()));
			generator.Define(symbolRotate,
				std::make_unique<RotateAction>(glm::vec3(0.0f, 0.0f, 45.0f)));

			EXPECT_THAT(
				generator.Generate(
					{ symbolDraw, symbolRotate, symbolDraw, symbolRotate, symbolDraw }),
				ElementsAre(
					Field(&MeshGroup::instances,
						ElementsAre(
							Field(&Transform::rotation, Eq(glm::vec3(0))),
							Field(&Transform::rotation, Eq(glm::vec3(0.0f, 0.0f, 45.0f))),
							Field(&Transform::rotation, Eq(glm::vec3(0.0f, 0.0f, 90.0f)))
						))));
		}

		TEST(LSystemMeshGeneratorTest, RotateChangesMoveDirection)
		{
			Symbol symbolDraw{ 'a' };
			Symbol symbolRotate{ 'b' };
			Symbol symbolMove{ 'c' };

			MeshGenerator generator;
			generator.Define(symbolDraw,
				std::make_unique<DrawAction>(
					std::make_unique<QuadDefinition>(),
					Material()));
			generator.Define(symbolRotate, std::make_unique<RotateAction>(glm::vec3(0.0f, 0.0f, 30.0f)));
			generator.Define(symbolMove, std::make_unique<MoveAction>());

			EXPECT_THAT(
				generator.Generate({
					symbolDraw,
					symbolRotate, symbolMove, symbolDraw,
					symbolRotate, symbolMove, symbolDraw }),
					ElementsAre(
						Field(&MeshGroup::instances,
							ElementsAre(
								Field(&Transform::position, Eq(glm::vec3(0))),
								Field(&Transform::position, Eq(glm::vec3(
									-glm::sin(glm::radians(30.0f)),
									glm::cos(glm::radians(30.0f)),
									0.0f))),
								Field(&Transform::position, Eq(glm::vec3(
									-glm::sin(glm::radians(30.0f)) - glm::sin(glm::radians(60.0f)),
									glm::cos(glm::radians(30.0f)) + glm::cos(glm::radians(60.0f)),
									0.0f)))
							))));
		}

		TEST(LSystemMeshGeneratorTest, SaveRestoreReturnsToEarlierState)
		{
			Symbol symbolDraw{ 'a' };
			Symbol symbolRotate{ 'b' };
			Symbol symbolMove{ 'c' };
			Symbol symbolSave{ 'd' };
			Symbol symbolRestore{ 'e' };

			MeshGenerator generator;
			generator.Define(symbolDraw,
				std::make_unique<DrawAction>(
					std::make_unique<QuadDefinition>(),
					Material()));
			generator.Define(symbolRotate,
				std::make_unique<RotateAction>(glm::vec3(0.0f, 0.0f, 30.0f)));
			generator.Define(symbolMove, std::make_unique<MoveAction>());
			generator.Define(symbolSave, std::make_unique<SaveAction>());
			generator.Define(symbolRestore, std::make_unique<RestoreAction>());

			EXPECT_THAT(
				generator.Generate({
					symbolSave, symbolDraw,
					symbolRotate, symbolMove, symbolDraw,
					symbolRestore, symbolDraw }),
					ElementsAre(
						Field(&MeshGroup::instances,
							ElementsAre(
								Field(&Transform::position, Eq(glm::vec3(0))),
								Field(&Transform::position, Eq(glm::vec3(
									-glm::sin(glm::radians(30.0f)),
									glm::cos(glm::radians(30.0f)),
									0.0f))),
								Field(&Transform::position, Eq(glm::vec3(0)))
							))));
		}
	}
}
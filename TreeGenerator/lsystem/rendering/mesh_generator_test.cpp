#include "mesh_generator.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

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
using ::testing::SizeIs;

namespace tree_generator::lsystem
{
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
			generator.DefineDraw(a, quad);

			// MeshData and Vertex don't have equality defined on them, so we
			// can't directly use equals. For now, assume that if the size is
			// the same, the vertices were set correctly.
			EXPECT_THAT(
				generator.Generate({ a }),
				ElementsAre(
					Field(&MeshGenerator::MeshGroup::mesh,
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
			generator.DefineDraw(a, quad);

			EXPECT_THAT(
				generator.Generate({ a, a }),
				ElementsAre(
					Field(&MeshGenerator::MeshGroup::instances, SizeIs(2))
				));
		}

		TEST(LSystemMeshGeneratorTest, MoveAdvancesTransform)
		{
			Symbol a{ 'a' };
			MeshData quad = CreateQuad();

			MeshGenerator generator;
			generator.DefineDraw(a, quad);
			generator.DefineMove(a);

			EXPECT_THAT(
				generator.Generate({ a, a }),
				ElementsAre(
					Field(&MeshGenerator::MeshGroup::instances,
						ElementsAre(
							Field(&Transform::position, Eq(glm::vec3(0))),
							Field(&Transform::position, Eq(glm::vec3(0.0f, 1.0f, 0.0f)))
						))));
		}
	}
}
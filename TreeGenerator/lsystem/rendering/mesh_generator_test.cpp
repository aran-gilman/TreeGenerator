#include "mesh_generator.h"

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
			generator.DefineDraw(a, quad);

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
			generator.DefineDraw(symbolDraw, CreateQuad());
			generator.DefineMove(symbolMove);

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
			generator.DefineDraw(symbolDraw, CreateQuad());
			generator.DefineRotate(symbolRotate, glm::vec3(0.0f, 0.0f, 45.0f));

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
			generator.DefineDraw(symbolDraw, CreateQuad());
			generator.DefineRotate(symbolRotate, glm::vec3(0.0f, 0.0f, 30.0f));
			generator.DefineMove(symbolMove);

			EXPECT_THAT(
				generator.Generate({ symbolDraw, symbolRotate, symbolMove, symbolDraw }),
				ElementsAre(
					Field(&MeshGroup::instances,
						ElementsAre(
							Field(&Transform::position, Eq(glm::vec3(0))),
							Field(&Transform::position, Eq(glm::vec3(
								glm::sin(glm::radians(30.0f)),
								glm::cos(glm::radians(30.0f)),
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
			generator.DefineDraw(symbolDraw, CreateQuad());
			generator.DefineRotate(symbolRotate, glm::vec3(0.0f, 0.0f, 30.0f));
			generator.DefineMove(symbolMove);
			generator.DefineSave(symbolSave);
			generator.DefineRestore(symbolRestore);

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
								glm::sin(glm::radians(30.0f)),
								glm::cos(glm::radians(30.0f)),
								0.0f))),
							Field(&Transform::position, Eq(glm::vec3(0)))
						))));
		}
	}
}
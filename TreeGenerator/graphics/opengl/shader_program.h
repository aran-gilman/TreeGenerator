#ifndef TREE_GENERATOR_OPENGL_SHADER_PROGRAM_H_
#define TREE_GENERATOR_OPENGL_SHADER_PROGRAM_H_

#include <memory>
#include <string>
#include <unordered_map>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "typed_shader.h"

namespace tree_generator
{
	namespace opengl
	{
		// A type-safe wrapper for OpenGL shader programs, with
		// ownership/memory managed via smart pointer.
		class ShaderProgram
		{
		public:
			static std::unique_ptr<ShaderProgram> Create(
				const VertexShader& vertexShader,
				const FragmentShader& fragmentShader);

			~ShaderProgram();

			void Bind() const;

			void BindUniformBlock(
				const std::string& uniformBlockName,
				GLuint uniformBlockBinding);

			void SetUniform(const std::string& uniform, glm::vec4 value);

		private:
			GLuint name_;
			std::unordered_map<std::string, int> uniformLocations_;

			ShaderProgram(GLuint name) : name_(name) {}
			int GetUniformLocation(const std::string& uniform);
		};
	}
}

#endif // !TREE_GENERATOR_GRAPHICS_OPENGL_SHADER_PROGRAM_H_
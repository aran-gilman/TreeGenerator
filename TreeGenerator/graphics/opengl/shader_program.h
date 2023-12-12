#ifndef TREE_GENERATOR_OPENGL_SHADER_PROGRAM_H_
#define TREE_GENERATOR_OPENGL_SHADER_PROGRAM_H_

#include <memory>
#include <string>

#include <glad/glad.h>

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

		private:
			ShaderProgram(GLuint name) : name_(name) {}
			GLuint name_;
		};
	}
}

#endif // !TREE_GENERATOR_GRAPHICS_OPENGL_SHADER_PROGRAM_H_
#ifndef TREE_GENERATOR_TYPED_SHADER_H_
#define TREE_GENERATOR_TYPED_SHADER_H_

#include <iostream>
#include <memory>
#include <string>

#include <glad/glad.h>

namespace tree_generator
{
	namespace opengl
	{
		enum class ShaderType : GLenum
		{
			Vertex = GL_VERTEX_SHADER,
			Fragment = GL_FRAGMENT_SHADER,
		};

		// A type-safe wrapper for OpenGL shaders, with ownership/memory
		// managed via smart pointer.
		template <ShaderType TShaderType>
		class TypedShader
		{
		public:
			static std::unique_ptr<TypedShader<TShaderType>> Create(
				const std::string& source)
			{
				unsigned int name = glCreateShader(static_cast<GLenum>(TShaderType));
				const char* sourceData = source.c_str();
				glShaderSource(name, 1, &sourceData, nullptr);
				glCompileShader(name);

				int success = 0;
				char infoLog[512];

				glGetShaderiv(name, GL_COMPILE_STATUS, &success);
				if (!success)
				{
					glGetShaderInfoLog(name, 512, nullptr, infoLog);
					std::cerr
						<< "Vertex shader compilation failed: "
						<< infoLog
						<< std::endl;
					return nullptr;
				}

				// Cleanup is performed via destructor instead of a custom
				// deleter because it makes the unique_ptr returned here a bit
				// easier to work with by simplifying its type.
				return std::unique_ptr<TypedShader>(new TypedShader(name));
			}

			~TypedShader()
			{
				glDeleteShader(name_);
			}

			GLuint Name() const { return name_; }

		private:
			TypedShader(GLuint name) : name_(name) {}
			GLuint name_;
		};
		
		using VertexShader = TypedShader<ShaderType::Vertex>;
		using FragmentShader = TypedShader<ShaderType::Fragment>;
	}
}

#endif  // !TREE_GENERATOR_TYPED_SHADER_H_
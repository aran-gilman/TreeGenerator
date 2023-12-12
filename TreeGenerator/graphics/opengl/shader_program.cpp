#include "shader_program.h"

namespace tree_generator
{
	namespace opengl
	{
		std::unique_ptr<ShaderProgram> ShaderProgram::Create(
			const VertexShader& vertexShader,
			const FragmentShader& fragmentShader)
		{
			GLuint name = glCreateProgram();
			glAttachShader(name, vertexShader.Name());
			glAttachShader(name, fragmentShader.Name());
			glLinkProgram(name);

			int success = 0;
			char infoLog[512];
			glGetProgramiv(name, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(name, 512, nullptr, infoLog);
				std::cerr << "Shader program linking failed: " << infoLog << std::endl;
			}
			return std::unique_ptr<ShaderProgram>(new ShaderProgram(name));
		}

		ShaderProgram::~ShaderProgram()
		{
			glDeleteProgram(name_);
		}

		void ShaderProgram::Bind() const
		{
			glUseProgram(name_);
		}

		void ShaderProgram::BindUniformBlock(
			const std::string& uniformBlockName,
			GLuint uniformBlockBinding)
		{
			GLuint index = glGetUniformBlockIndex(name_, uniformBlockName.c_str());
			glUniformBlockBinding(name_, index, uniformBlockBinding);
		}
	}
}

#include "shader_program.h"

#include <glm/gtc/type_ptr.hpp>

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

		void ShaderProgram::SetUniform(const std::string& uniform, glm::vec4 value)
		{
			glUniform4fv(GetUniformLocation(uniform), 1, glm::value_ptr(value));
		}

		int ShaderProgram::GetUniformLocation(const std::string& uniform)
		{
			auto iter = uniformLocations_.find(uniform);
			if (iter == uniformLocations_.end())
			{
				auto res = uniformLocations_.emplace(
					uniform, glGetUniformLocation(name_, uniform.c_str()));
				iter = res.first;
			}
			return iter->second;
		}
	}
}

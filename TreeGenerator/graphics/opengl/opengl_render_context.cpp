#include "opengl_render_context.h"

#include <iostream>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../common/camera.h"
#include "../common/window.h"

#include "internal/opengl_camera.h"
#include "internal/opengl_mesh_renderer.h"
#include "internal/typed_shader.h"
#include "internal/shader_program.h"

namespace tree_generator::opengl
{
	const char* vertexShaderSource = R"s(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

// Model matrix (per instance)
layout (location = 3) in vec4 aModel0;
layout (location = 4) in vec4 aModel1;
layout (location = 5) in vec4 aModel2;
layout (location = 6) in vec4 aModel3;

layout (std140) uniform Camera
{
	mat4 view;
	mat4 projection;
} camera;

out VS_OUT
{
	vec3 normal;
} vs_out;

void main()
{
	mat4 model = mat4(aModel0, aModel1, aModel2, aModel3);
	gl_Position = camera.projection * camera.view * model * vec4(aPos, 1.0f);
	vs_out.normal = aNormal;
})s";

	const char* normalFragmentShaderSource = R"s(
#version 330 core

in VS_OUT
{
	vec3 normal;
} vs_out;

out vec4 FragColor;

void main()
{
	FragColor = vec4(abs(vs_out.normal), 1.0f);
})s";

	const char* materialFragmentShaderSource = R"s(
#version 330 core

struct Material
{
    vec4 color;
};

uniform Material material;

out vec4 FragColor;

void main()
{
	FragColor = material.color;
})s";
	template <typename T>
	std::unique_ptr<T> ThrowIfNull(std::unique_ptr<T> value, const std::string& message)
	{
		if (value == nullptr)
		{
			throw std::runtime_error(message);
		}
		return std::move(value);
	}

	OpenGLRenderContext::OpenGLRenderContext()
	{
		int version = gladLoadGL();
		if (version == 0)
		{
			throw std::runtime_error("Failed to load OpenGL");
		}

		glEnable(GL_DEPTH_TEST);

		std::unique_ptr<VertexShader> vertexShader = ThrowIfNull(
			VertexShader::Create(vertexShaderSource),
			"Vertex shader compilation failed");

		std::unique_ptr<FragmentShader> normalFragmentShader = ThrowIfNull(
			FragmentShader::Create(normalFragmentShaderSource),
			"Normal fragment shader compilation failed");

		std::unique_ptr<FragmentShader> materialFragmentShader = ThrowIfNull(
			FragmentShader::Create(materialFragmentShaderSource),
			"Material fragment shader compilation failed");

		normalShader_ = ThrowIfNull(
			ShaderProgram::Create(*vertexShader, *normalFragmentShader),
			"Normal shader linking failed");
		materialShader_ = ThrowIfNull(
			ShaderProgram::Create(*vertexShader, *materialFragmentShader),
			"Material shader linking failed");

		normalShader_->BindUniformBlock("Camera", 1);
		materialShader_->BindUniformBlock("Camera", 1);
	}

	OpenGLRenderContext::~OpenGLRenderContext()
	{
	}

	std::unique_ptr<Camera> OpenGLRenderContext::CreateCamera()
	{
		return std::make_unique<OpenGLCamera>(1);
	}

	std::unique_ptr<MeshRenderer> OpenGLRenderContext::CreateMeshRenderer()
	{
		return std::make_unique<OpenGLMeshRenderer>(
			materialShader_.get(), normalShader_.get());
	}
}


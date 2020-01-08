#include "inpch.h"


namespace Infinit {

	std::shared_ptr<Shader> Shader::DefaultShader;

	std::shared_ptr<Shader> Shader::Create(const string& filePath)
	{
		IN_CORE_ASSERT((bool)RendererAPI::Renderer, "No RenderAPI selected");
		switch (RendererAPI::Renderer)
		{
		case RendererAPI::Type::OpenGL:return std::shared_ptr<OpenGLShader>(new OpenGLShader(filePath));
		}

		return nullptr;
	}

	std::shared_ptr<Shader> Shader::Create(const string& vertexSource, const string& fragmentSource)
	{
		IN_CORE_ASSERT((bool)RendererAPI::Renderer, "No RenderAPI selected");
		switch (RendererAPI::Renderer)
		{
		case RendererAPI::Type::OpenGL:return std::shared_ptr<OpenGLShader>(new OpenGLShader(vertexSource, fragmentSource));
		}

		return nullptr;
	}

	std::shared_ptr<Shader> Shader::Create(const json& json_object)
	{
		IN_CORE_ASSERT((bool)RendererAPI::Renderer, "No RenderAPI selected");
		switch (RendererAPI::Renderer)
		{
		case RendererAPI::Type::OpenGL: { std::shared_ptr<OpenGLShader> result = std::make_shared<OpenGLShader>(); result->Deserialize(json_object); return result; }
		}

		return nullptr;
	}

}
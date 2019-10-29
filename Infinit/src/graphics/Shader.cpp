#include "inpch.h"
#include "Shader.h"

#include "Platform/OpenGL/OpenGLShader.h"

#include <graphics/RendererAPI.h>
#include "Core/Log.h"


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

}
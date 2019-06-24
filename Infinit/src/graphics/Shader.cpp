#include "inpch.h"
#include "Shader.h"

#include "Platform/OpenGL/OpenGLShader.h"

#include <graphics/RendererAPI.h>
#include "Core/Log.h"


namespace Infinit {

	Shader* Shader::Create(const string& vertexSource, const string& fragmentSource)
	{
		IN_CORE_ASSERT((bool)RendererAPI::Renderer, "No RenderAPI selected");
		switch (RendererAPI::Renderer)
		{
		case RendererAPI::Type::OpenGL:return new OpenGLShader(vertexSource, fragmentSource);
		}

		return nullptr;
	}

}
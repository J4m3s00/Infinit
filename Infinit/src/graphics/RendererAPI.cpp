#include "inpch.h"
#include "RendererAPI.h"

#include "Core/Log.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Infinit {

	RendererAPI::Type RendererAPI::Renderer = RendererAPI::Type::None;
	RendererAPI* RendererAPI::s_Instance = nullptr;

	void RendererAPI::Init()
	{
		IN_CORE_ASSERT((bool)Renderer, "No Renderer selected");
		switch (Renderer)
		{
		case Type::OpenGL: s_Instance = new OpenGLRendererAPI();
		}
	}

}
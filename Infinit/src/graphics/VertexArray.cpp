#include "inpch.h"
namespace Infinit {

	VertexArray* VertexArray::Create()
	{
		IN_CORE_ASSERT((bool)RendererAPI::Renderer, "No RenderAPI selected");
		switch (RendererAPI::Renderer)
		{
		case RendererAPI::Type::OpenGL:return new OpenGLVertexArray();
		}

		return nullptr;
	}

}
#include "inpch.h"
#include "Buffer.h"

#include <Platform/OpenGL/OpenGLBuffer.h>
#include <graphics/RendererAPI.h>

namespace Infinit {

	VertexBuffer* VertexBuffer::Create(const void* data, uint size)
	{
		IN_CORE_ASSERT((bool)RendererAPI::Renderer, "No RenderAPI selected");
		switch (RendererAPI::Renderer)
		{
		case RendererAPI::Type::OpenGL:return new OpenGLVertexBuffer(data, size);
		}
		
		return nullptr;
	}

	IndexBuffer* IndexBuffer::Create(const uint* data, uint count)
	{
		IN_CORE_ASSERT((bool)RendererAPI::Renderer, "No RenderAPI selected");
		switch (RendererAPI::Renderer)
		{
		case RendererAPI::Type::OpenGL:return new OpenGLIndexBuffer(data, count);
		}
		return nullptr;
	}

}
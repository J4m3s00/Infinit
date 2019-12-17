#include "inpch.h"

namespace Infinit {

	FrameBuffer* FrameBuffer::Create(uint width, uint height, FramebufferFormat format)
	{
		IN_CORE_ASSERT((bool)RendererAPI::Renderer, "No RendererAPI selected!");
		switch (RendererAPI::Renderer)
		{
		case RendererAPI::Type::OpenGL: return new OpenGLFrameBuffer(width, height, format);
		}
		return nullptr;
	}

}
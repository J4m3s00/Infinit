#include "inpch.h"
#include "Buffer.h"

#include <Platform/OpenGL/OpenGLBuffer.h>

namespace Infinit {

	VertexBuffer* VertexBuffer::Create(const void* data, uint size)
	{
		return new OpenGLVertexBuffer(data, size);
	}

	IndexBuffer* IndexBuffer::Create(const uint* data, uint count)
	{
		return new OpenGLIndexBuffer(data, count);
	}

}
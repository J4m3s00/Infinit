#pragma once

#include "Core/Core.h"

namespace Infinit {

	class INFINIT_API BufferElement
	{
		
	};

	class INFINIT_API BufferLayout
	{

	};

	class INFINIT_API VertexBuffer
	{
	public:
		virtual ~VertexBuffer() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		static VertexBuffer* Create(const void* data, uint size);
	};

	class INFINIT_API IndexBuffer
	{
	public:
		virtual ~IndexBuffer() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		static IndexBuffer* Create(const uint* indices, uint count);
	};

}
#pragma once

#include "Core/Core.h"
#include "Buffer.h"

namespace Infinit {

	class INFINIT_API VertexArray
	{
	public:
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) = 0;
		virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const = 0;
		virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) = 0;

		static VertexArray* Create();
	};

}
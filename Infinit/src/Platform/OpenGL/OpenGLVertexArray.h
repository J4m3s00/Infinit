#pragma once

#include "Core/Core.h"
#include "graphics/VertexArray.h"

namespace Infinit {

	class INFINIT_API OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray();
		virtual ~OpenGLVertexArray();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) override;
		virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const override { return m_IndexBuffer; }
		virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) override;
	private:
		uint m_VertexBufferIndex = 0;
		uint m_RendererID = 0;
		std::vector<std::shared_ptr<VertexBuffer>> m_VertexBuffers;
		std::shared_ptr<IndexBuffer> m_IndexBuffer;
	};

}
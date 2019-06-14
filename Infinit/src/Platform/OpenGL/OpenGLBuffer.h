#pragma once

#include "Core/Core.h"
#include "graphics/Buffer.h"

namespace Infinit {

	class INFINIT_API OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(const void* data, uint size);
		~OpenGLVertexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;
	private:
		uint m_RendererID = 0;
	};

	class INFINIT_API OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(const uint* data, uint count);
		~OpenGLIndexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;
	private:
		uint m_RendererID = 0;
	};

}
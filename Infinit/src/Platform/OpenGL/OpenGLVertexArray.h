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
	private:
		uint m_RendererID;
	};

}
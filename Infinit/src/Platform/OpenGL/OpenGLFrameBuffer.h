#pragma once

#include "graphics/FrameBuffer.h"

namespace Infinit {

	class OpenGLFrameBuffer : public FrameBuffer
	{
	public:
		OpenGLFrameBuffer();
		virtual ~OpenGLFrameBuffer();

		void Bind() const override;
		void Unbind() const override;
	private:
		uint m_ID;
	};

}
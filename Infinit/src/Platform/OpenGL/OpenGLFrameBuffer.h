#pragma once

#include "graphics/FrameBuffer.h"

namespace Infinit {

	class OpenGLFrameBuffer : public FrameBuffer
	{
	public:
		OpenGLFrameBuffer(uint width, uint height, FramebufferFormat format);
		virtual ~OpenGLFrameBuffer();

		void Resize(uint width, uint height) override;

		void Bind() const override;
		void Unbind() const override;

		void BindTexture(uint slot = 0) const override;

		uint GetColorAttachment() const override { return m_ColorAttachment; }
	private:
		uint m_RendererID;
		uint m_Width;
		uint m_Height;
		FramebufferFormat m_Format;
		uint m_ColorAttachment;
		uint m_DepthAttachment;
	};

}
#pragma once

#include "Core/Core.h"

namespace Infinit {

	enum class FramebufferFormat
	{
		None = 0,
		RGBA8 = 1,
		RGBA16F = 2
	};

	class FrameBuffer
	{
	public:
		virtual ~FrameBuffer() {}

		virtual void Resize(uint width, uint height) = 0;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void BindTexture(uint slot = 0) const = 0;

		virtual uint GetColorAttachment() const { return 0; }

		static FrameBuffer* Create(uint width, uint height, FramebufferFormat format);
	};

}
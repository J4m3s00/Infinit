#pragma once

#include "Core/Core.h"

namespace Infinit {

	class FrameBuffer
	{
	public:
		virtual ~FrameBuffer() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		static FrameBuffer* Create();
	};

}
#pragma once

#include "Core/Core.h"

namespace Infinit {
	typedef void(*RenderCommandFn)(void*);

	class RenderCommandQueue
	{
	public:
		RenderCommandQueue();
		~RenderCommandQueue();

		void* Allocate(RenderCommandFn func, uint size);

		void Execute();
	private:
		byte* m_CommandBuffer;
		byte* m_CommandBufferPtr;
		uint m_CommandCount = 0;
	};

}
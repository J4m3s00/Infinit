#pragma once

#include "Core/Core.h"

namespace Infinit {

	class RenderCommandQueue
	{
	public:
		RenderCommandQueue();
		~RenderCommandQueue();

		typedef void(*RenderCommandFn)(void*);

		void* Allocate(RenderCommandFn func, uint size);

		void Execute();
	private:
		byte* m_CommandBuffer;
		byte* m_CommandBufferPtr;
		uint m_CommandCount = 0;
	};

}
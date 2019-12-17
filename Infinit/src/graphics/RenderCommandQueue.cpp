#include <inpch.h>
//10mb buffersize
#define COMMAND_BUFFER_SIZE (10 * 1024 * 1024)

namespace Infinit {

	RenderCommandQueue::RenderCommandQueue()
	{
		m_CommandBuffer = new byte[COMMAND_BUFFER_SIZE];
		m_CommandBufferPtr = m_CommandBuffer;
		memset(m_CommandBuffer, 0, COMMAND_BUFFER_SIZE);
	}

	RenderCommandQueue::~RenderCommandQueue()
	{
		delete[] m_CommandBuffer;
	}

	void* RenderCommandQueue::Allocate(RenderCommandFn fn, uint size)
	{
		*(RenderCommandFn*)m_CommandBufferPtr = fn;
		m_CommandBufferPtr += sizeof(RenderCommandFn);

		*(uint*)m_CommandBufferPtr = size;
		m_CommandBufferPtr += sizeof(uint);

		void* memory = m_CommandBufferPtr;
		m_CommandBufferPtr += size;

		m_CommandCount++;
		return memory;
	}

	void RenderCommandQueue::Execute()
	{
		byte* buffer = m_CommandBuffer;

		for (uint i = 0; i < m_CommandCount; i++)
		{
			RenderCommandFn function = *(RenderCommandFn*)buffer;
			buffer += sizeof(RenderCommandFn);

			uint size = *(uint*)buffer;
			buffer += sizeof(uint);
			function(buffer);
			buffer += size;
		}

		m_CommandBufferPtr = m_CommandBuffer;
		m_CommandCount = 0;
	}
}
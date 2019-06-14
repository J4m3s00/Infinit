#include "inpch.h"
#include "OpenGLBuffer.h"

#include <glad/glad.h>

namespace Infinit {
	
	OpenGLVertexBuffer::OpenGLVertexBuffer(const void* data, uint size)
	{
		glGenBuffers(1, &m_RendererID);
		Bind();
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		glDeleteBuffers(1, &m_RendererID);
	}

	void OpenGLVertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	}

	void OpenGLVertexBuffer::Unbind() const
	{
#ifdef IN_DEBUG
		glBindBuffer(GL_ARRAY_BUFFER, 0);
#endif
	}

	OpenGLIndexBuffer::OpenGLIndexBuffer(const uint* data, uint count)
	{
		glGenBuffers(1, &m_RendererID);
		Bind();
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint), data, GL_STATIC_DRAW);
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		glDeleteBuffers(1, &m_RendererID);
	}

	void OpenGLIndexBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
	}

	void OpenGLIndexBuffer::Unbind() const
	{
#ifdef IN_DEBUG
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
#endif
	}

}
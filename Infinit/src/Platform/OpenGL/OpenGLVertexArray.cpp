#include "inpch.h"
#include "OpenGLVertexArray.h"

#include <glad/glad.h>

namespace Infinit {

	OpenGLVertexArray::OpenGLVertexArray()
	{
		glGenVertexArrays(1, &m_RendererID);
		Bind();
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &m_RendererID);
	}

	void OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(m_RendererID);
	}

	void OpenGLVertexArray::Unbind() const
	{
#ifdef IN_DEBUG
		glBindVertexArray(0);
#endif
	}

}
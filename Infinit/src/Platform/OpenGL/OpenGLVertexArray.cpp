#include "inpch.h"
#include "OpenGLVertexArray.h"

#include <glad/glad.h>

namespace Infinit {

	static GLenum ShaderDataTypeToOpenGLType(ShaderDataType type)
	{
		switch (type)
		{
		case Infinit::ShaderDataType::Float:	return GL_FLOAT;
		case Infinit::ShaderDataType::Float2:	return GL_FLOAT;
		case Infinit::ShaderDataType::Float3:	return GL_FLOAT;
		case Infinit::ShaderDataType::Float4:	return GL_FLOAT;
		case Infinit::ShaderDataType::Matrix3:	return GL_FLOAT;
		case Infinit::ShaderDataType::Matrix4:	return GL_FLOAT;
		case Infinit::ShaderDataType::Int:		return GL_INT;
		case Infinit::ShaderDataType::Int2:		return GL_INT;
		case Infinit::ShaderDataType::Int3:		return GL_INT;
		case Infinit::ShaderDataType::Int4:		return GL_INT;
		case Infinit::ShaderDataType::UInt:		return GL_UNSIGNED_INT;
		case Infinit::ShaderDataType::Byte4:	return GL_UNSIGNED_BYTE;
		case Infinit::ShaderDataType::Bool:		return GL_BOOL;
		}

		IN_CORE_ASSERT(false, "Unknown Shader Type!");
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		glGenVertexArrays(1, &m_RendererID);
		glBindVertexArray(m_RendererID);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		IN_CORE_INFO("VertexArray Deleted!");
		glDeleteVertexArrays(1, &m_RendererID);
	}

	void OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(m_RendererID);
	}

	void OpenGLVertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
	{
		IN_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "No Vertex Layout set for the buffer");
		glBindVertexArray(m_RendererID);
		vertexBuffer->Bind();

		uint index = 0;
		const auto& layout = vertexBuffer->GetLayout();
		for (const auto& element : layout)
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index, 
						element.GetComponentCount(), 
						ShaderDataTypeToOpenGLType(element.Type), 
						element.Normalized ? GL_TRUE : GL_FALSE, 
						layout.GetStride(), (const void*)element.Offset);
			index++;
		}

		m_VertexBuffers.push_back(vertexBuffer);
	}

	void OpenGLVertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
	{
		glBindVertexArray(m_RendererID);
		indexBuffer->Bind();

		m_IndexBuffer = indexBuffer;
	}

}
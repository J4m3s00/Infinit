#include "inpch.h"

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
		IN_RENDER_S({
			glGenVertexArrays(1, &self->m_RendererID);
			glBindVertexArray(self->m_RendererID); 
			})
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		IN_CORE_INFO("OpenGLVertexArray {0} Destroyed!", m_RendererID);
		IN_RENDER_S({
			glDeleteVertexArrays(1, &self->m_RendererID);
			})
	}

	void OpenGLVertexArray::Bind() const
	{
		IN_RENDER_S({
		glBindVertexArray(self->m_RendererID);
		if (self->m_IndexBuffer)
			self->m_IndexBuffer->Bind();
			});
	}

	void OpenGLVertexArray::Unbind() const
	{
		IN_RENDER_S({
		self->m_IndexBuffer->Unbind();
		glBindVertexArray(0);
			});
	}

	void OpenGLVertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
	{
		IN_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "No Vertex Layout set for the buffer");
		m_VertexBuffers.push_back(vertexBuffer);
		Bind();
		vertexBuffer->Bind();

		IN_RENDER_S1(vertexBuffer, {

		const auto & layout = vertexBuffer->GetLayout();
		for (const auto& element : layout)
		{
			glEnableVertexAttribArray(self->m_VertexBufferIndex);
			glVertexAttribPointer(self->m_VertexBufferIndex,
						element.GetComponentCount(),
						ShaderDataTypeToOpenGLType(element.Type),
						element.Normalized ? GL_TRUE : GL_FALSE,
						layout.GetStride(), (const void*)element.Offset);
			self->m_VertexBufferIndex++;
		}
			});

	}

	void OpenGLVertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
	{
		m_IndexBuffer = indexBuffer;

		IN_RENDER_S({
			glBindVertexArray(self->m_RendererID);
			if (self->m_IndexBuffer)
				self->m_IndexBuffer->Bind();
			});

	}

}
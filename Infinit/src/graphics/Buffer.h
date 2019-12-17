#pragma once

namespace Infinit {

	enum class ShaderDataType
	{
		None = 0, Float, Float2, Float3, Float4, Matrix3, Matrix4, Int, Int2, Int3, Int4, UInt, Byte4, Bool, Texture2D, TextureCube
	};

	static uint ShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
		case Infinit::ShaderDataType::Float:			return 4;
		case Infinit::ShaderDataType::Float2:			return 4 * 2;
		case Infinit::ShaderDataType::Float3:			return 4 * 3;
		case Infinit::ShaderDataType::Float4:			return 4 * 4;
		case Infinit::ShaderDataType::Matrix3:			return 4 * 3 * 3;
		case Infinit::ShaderDataType::Matrix4:			return 4 * 4 * 4;
		case Infinit::ShaderDataType::Int:				return 4;
		case Infinit::ShaderDataType::Texture2D:		return 4;
		case Infinit::ShaderDataType::TextureCube:		return 4;
		case Infinit::ShaderDataType::Int2:				return 4 * 2;
		case Infinit::ShaderDataType::Int3:				return 4 * 3;
		case Infinit::ShaderDataType::Int4:				return 4 * 4;
		case Infinit::ShaderDataType::UInt:				return 4;
		case Infinit::ShaderDataType::Byte4:			return 4;
		case Infinit::ShaderDataType::Bool:				return 1;
		}

		IN_CORE_WARN("Unknown ShaderDataType");
		return 0;
	}

	static ShaderDataType ShaderDataTypeFromString(const string& typeString)
	{
		if (typeString == "float") return ShaderDataType::Float;
		else if (typeString == "vec2") return ShaderDataType::Float2;
		else if (typeString == "vec3") return ShaderDataType::Float3;
		else if (typeString == "vec4") return ShaderDataType::Float4;
		else if (typeString == "bool") return ShaderDataType::Bool;
		else if (typeString == "dont know") return ShaderDataType::Byte4;
		else if (typeString == "int") return ShaderDataType::Int;
		else if (typeString == "ivec2") return ShaderDataType::Int2;
		else if (typeString == "ivec3") return ShaderDataType::Int3;
		else if (typeString == "ivec4") return ShaderDataType::Int4;
		else if (typeString == "mat3") return ShaderDataType::Matrix3;
		else if (typeString == "mat4") return ShaderDataType::Matrix4;
		else if (typeString == "sampler2D") return ShaderDataType::Texture2D;
		else if (typeString == "samplerCube") return ShaderDataType::TextureCube;

		return ShaderDataType::None;
	}

	struct BufferElement
	{
		ShaderDataType Type;
		string Name;
		uint Size;
		uint Offset;
		bool Normalized;

		BufferElement() {}

		BufferElement(ShaderDataType type, const string& name, bool normalized = false)
			: Name(name), Type(type), Normalized(normalized), Size(ShaderDataTypeSize(type)), Offset(0)
		{}

		uint GetComponentCount() const
		{
			switch (Type)
			{
			case Infinit::ShaderDataType::Float:	return 1;
			case Infinit::ShaderDataType::Float2:	return 2;
			case Infinit::ShaderDataType::Float3:	return 3;
			case Infinit::ShaderDataType::Float4:	return 4;
			case Infinit::ShaderDataType::Matrix3:	return 3 * 3;
			case Infinit::ShaderDataType::Matrix4:	return 4 * 4;
			case Infinit::ShaderDataType::Int:		return 1;
			case Infinit::ShaderDataType::Int2:		return 2;
			case Infinit::ShaderDataType::Int3:		return 3;
			case Infinit::ShaderDataType::Int4:		return 4;
			case Infinit::ShaderDataType::UInt:		return 1;
			case Infinit::ShaderDataType::Byte4:	return 4;
			case Infinit::ShaderDataType::Bool:		return 1;
			}

			IN_CORE_ASSERT(false, "Unknown Shader Type");
			return 0;
		}
	};

	class BufferLayout
	{
	public:
		BufferLayout() {}

		BufferLayout(const std::initializer_list<BufferElement>& elements)
			: m_Elements(elements)
		{
			CalculateOffsetAndStride();
		}

		inline uint GetStride() const { return m_Stride; }
		inline const std::vector<BufferElement>& GetElements() const { return m_Elements; }

		std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
		std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
		std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }
	private:
		void CalculateOffsetAndStride()
		{
			uint offset = 0;
			m_Stride = 0;
			for (auto& element : m_Elements)
			{
				element.Offset = offset;
				offset += element.Size;
				m_Stride += element.Size;
			}
		}
	private:
		std::vector<BufferElement> m_Elements;
		uint m_Stride = 0;
	};

	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual const BufferLayout& GetLayout() const = 0;
		virtual void SetLayout(const BufferLayout& layout) = 0;

		static VertexBuffer* Create(const void* data, uint size);
	};

	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual uint GetCount() const = 0;

		static IndexBuffer* Create(const uint* indices, uint count);
	};

}
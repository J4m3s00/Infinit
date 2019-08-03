#pragma once

#include "inpch.h"
#include "Core/Core.h"
#include "Core/Resource.h"
#include "graphics/Shader.h"
#include "graphics/Texture.h"
#include "graphics/Buffer.h"

namespace Infinit {

	enum class MaterialParameterType
	{
		None = 0, Float, Float2, Float3, Float4, Color3, Color4, Int, Bool, Texture2D, TextureCube
	};

	class MaterialParameter
	{
	public:
		MaterialParameter(const string& name, MaterialParameterType type, void* data)
			: m_Name(name), m_Type(type), m_Buffer(data)
		{}

		void UploadToShader(std::shared_ptr<Shader> shader) const;
		const string& GetName() const { return m_Name; }
		MaterialParameterType GetDataType() { return m_Type; }
		void* GetBufferPointer() { return m_Buffer; }
	private:
		void* m_Buffer;
		MaterialParameterType m_Type;
		string m_Name;
	};

	class Material
	{
	public:
		Material(std::shared_ptr<Shader> shader);
		virtual ~Material();

		void Bind() const;

		void AddTexture(const string& shaderName, std::shared_ptr<Texture2D> texture);
		void AddTexture(const string& shaderName, std::shared_ptr<TextureCube> texture);
		void AddParameter(MaterialParameter* param);

		void DrawImGui();
	public:
		std::shared_ptr<Shader> ShaderProgram;
	private:
		std::unordered_map<string, std::shared_ptr<Texture>> m_Textures;
		std::vector<MaterialParameter*> m_Params;
	};

}
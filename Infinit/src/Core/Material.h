#pragma once

#include "inpch.h"
#include "Core/Core.h"
#include "Core/Resource.h"
#include "graphics/Shader.h"
#include "graphics/Texture.h"
#include "graphics/Buffer.h"

namespace Infinit {

	class MaterialParameter
	{
	public:
		MaterialParameter(const string& name, ShaderDataType type, void* data)
			: m_Name(name), m_Type(type), m_Buffer(data)
		{}

		void UploadToShader(std::shared_ptr<Shader> shader) const;
	private:
		void* m_Buffer;
		ShaderDataType m_Type;
		string m_Name;
	};

	class Material
	{
	public:
		Material(std::shared_ptr<Shader> shader);
		virtual ~Material();

		void Bind() const;

		void AddTexture(const string& shaderName, std::shared_ptr<Texture> texture);
		void AddParameter(MaterialParameter* param);
	public:
		std::shared_ptr<Shader> ShaderProgram;
	private:
		std::unordered_map<string, std::shared_ptr<Texture>> m_Textures;
		std::vector<MaterialParameter*> m_Params;
	};

}
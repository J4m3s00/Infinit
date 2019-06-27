#include "inpch.h"
#include "Material.h"

#include "Core/Log.h"

namespace Infinit {

	void MaterialParameter::UploadToShader(std::shared_ptr<Shader> shader) const
	{
		switch (m_Type)
		{
		case Infinit::ShaderDataType::Float:	shader->SetUniform1f(m_Name, *(const float*)m_Buffer); break;
		case Infinit::ShaderDataType::Float2:	shader->SetUniform2f(m_Name, *(const glm::vec2*)m_Buffer); break;
		case Infinit::ShaderDataType::Float3:	shader->SetUniform3f(m_Name, *(const glm::vec3*) m_Buffer); break;
		case Infinit::ShaderDataType::Float4:	shader->SetUniform4f(m_Name, *(const glm::vec4*) m_Buffer); break;
		case Infinit::ShaderDataType::Int:		shader->SetUniform1i(m_Name, *(const int*)m_Buffer); break;
		case Infinit::ShaderDataType::Bool:		shader->SetUniform1i(m_Name, *(const bool*)m_Buffer); break;
		default: IN_ERROR("Unsupported data type for material parameter!"); break;
		}
		
	}

	Material::Material(std::shared_ptr<Shader> shader)
		: ShaderProgram(shader)
	{

	}

	Material::~Material()
	{
		for (MaterialParameter* p : m_Params)
			delete p;
	}

	void Material::Bind() const
	{
		IN_CORE_ASSERT(ShaderProgram, "Pls provide a valid shader for the Material");
		ShaderProgram->Bind();
		for (auto tex : m_Textures)
		{
			uint slot = ShaderProgram->GetResourceSlot(tex.first);
			if (slot == -1) IN_CORE_WARN("Could not find texture {0} in shader", tex.first);

			ShaderProgram->SetUniform1i(tex.first, slot);
			tex.second->Bind(slot);
		}
		for (MaterialParameter* param : m_Params)
		{
			param->UploadToShader(ShaderProgram);
		}
	}

	void Material::AddTexture(const string& shaderName, std::shared_ptr<Texture> texture)
	{
		m_Textures[shaderName] = texture;
	}

	void Material::AddParameter( MaterialParameter* param)
	{
		m_Params.push_back(param);
	}

}
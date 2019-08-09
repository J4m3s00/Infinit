#include "inpch.h"
#include "Material.h"

#include "Core/Log.h"

#include "Core/ImGui/ImGuiHelper.h"
#include "Core/Application.h"

namespace Infinit {

	void MaterialParameter::UploadToShader(std::shared_ptr<Shader> shader) const
	{
		switch (m_Type)
		{
		case Infinit::MaterialParameterType::Float:			shader->SetUniform1f(m_Name, *(const float*)m_Buffer); break;
		case Infinit::MaterialParameterType::Float2:		shader->SetUniform2f(m_Name, *(const glm::vec2*)m_Buffer); break;
		case Infinit::MaterialParameterType::Float3:
		case Infinit::MaterialParameterType::Color3:		shader->SetUniform3f(m_Name, *(const glm::vec3*) m_Buffer); break;
		case Infinit::MaterialParameterType::Float4:
		case Infinit::MaterialParameterType::Color4:		shader->SetUniform4f(m_Name, *(const glm::vec4*) m_Buffer); break;
		case Infinit::MaterialParameterType::Int:			shader->SetUniform1i(m_Name, *(const int*)m_Buffer); break;
		case Infinit::MaterialParameterType::Bool:			shader->SetUniform1i(m_Name, *(const bool*)m_Buffer); break;
		case Infinit::MaterialParameterType::Texture2D:
		case Infinit::MaterialParameterType::TextureCube:	shader->SetUniform1i(m_Name, *(const uint*)m_Buffer); break;
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
		//for (auto tex : m_Textures)
		//{
		//	uint slot = ShaderProgram->GetResourceSlot(tex.first);
		//	if (slot == -1) IN_CORE_WARN("Could not find texture {0} in shader", tex.first);
		//
		//	ShaderProgram->SetUniform1i(tex.first, slot);
		//	tex.second->Bind(slot);
		//}
		for (MaterialParameter* param : m_Params)
		{
			param->UploadToShader(ShaderProgram);
			if (param->GetDataType() == MaterialParameterType::Texture2D || param->GetDataType() == MaterialParameterType::TextureCube)
			{
				const string& name = param->GetName();
				std::shared_ptr<Texture> tex = m_Textures.at(name);
				if (!tex)
				{
					IN_CORE_ERROR("Could not find Texture {0}!", name);
					continue;
				}
				uint slot = ShaderProgram->GetResourceSlot(name);
				tex->Bind(slot);
			}
		}
	}

	void Material::AddTexture(const string& shaderName, std::shared_ptr<Texture2D> texture)
	{
		m_Params.push_back(new MaterialParameter(shaderName, MaterialParameterType::Texture2D, (void*)new int(ShaderProgram->GetResourceSlot(shaderName))));
		m_Textures[shaderName] = texture;
	}

	void Material::AddTexture(const string& shaderName, std::shared_ptr<TextureCube> texture)
	{
		m_Params.push_back(new MaterialParameter(shaderName, MaterialParameterType::TextureCube, (void*)new int(ShaderProgram->GetResourceSlot(shaderName))));
		m_Textures[shaderName] = texture;
	}

	void Material::AddParameter( MaterialParameter* param)
	{
		m_Params.push_back(param);
	}

	void Material::DrawImGui()
	{
		if (ImGui::TreeNode(("Shader " + ShaderProgram->GetFilePath()).c_str()))
		{
			std::string buttonName = "Reload##" + ShaderProgram->GetFilePath();
			if (ImGui::Button(buttonName.c_str()))
				ShaderProgram->Reload("");
			ImGui::TreePop();
		}

		for (auto& param : m_Params)
		{
			switch (param->GetDataType())
			{
			case MaterialParameterType::Float:			ImGuiProperty(param->GetName(), *(float*)param->GetBufferPointer()); break;
			case MaterialParameterType::Float2:			ImGuiProperty(param->GetName(), *(glm::vec2*)param->GetBufferPointer()); break;
			case MaterialParameterType::Float3:			ImGuiProperty(param->GetName(), *(glm::vec3*)param->GetBufferPointer()); break;
			case MaterialParameterType::Float4:			ImGuiProperty(param->GetName(), *(glm::vec4*)param->GetBufferPointer()); break;
			case MaterialParameterType::Int:			ImGuiProperty(param->GetName(), *(int*)param->GetBufferPointer()); break;
			case MaterialParameterType::Bool:			ImGuiProperty(param->GetName(), *(bool*)param->GetBufferPointer()); break;
			case MaterialParameterType::Texture2D:		
			case MaterialParameterType::TextureCube:	ImGuiProperty(param->GetName(), m_Textures.at(param->GetName())); break;
			case MaterialParameterType::Color3:			ImGuiProperty(param->GetName(), *(glm::vec3*)param->GetBufferPointer(), PropertyFlags::ColorProperty); break;
			case MaterialParameterType::Color4:			ImGuiProperty(param->GetName(), *(glm::vec4*)param->GetBufferPointer(), PropertyFlags::ColorProperty); break;
			}
		}

		//for (auto& a : m_Textures)
		//{
		//	if (ImGui::CollapsingHeader(a.first.c_str(), nullptr, ImGuiTreeNodeFlags_DefaultOpen))
		//	{
		//		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 10));
		//		if (a.second)
		//		ImGui::Image((void*)a.second->GetRendererID(), ImVec2(64, 64));
		//		ImGui::PopStyleVar();
		//		if (ImGui::IsItemHovered())
		//		{
		//			if (a.second)
		//			{
		//				ImGui::BeginTooltip();
		//				ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		//				ImGui::TextUnformatted(a.second->GetPath().c_str());
		//				ImGui::PopTextWrapPos();
		//				ImGui::Image((void*)a.second->GetRendererID(), ImVec2(384, 384));
		//				ImGui::EndTooltip();
		//			}
		//			if (ImGui::IsItemClicked())
		//			{
		//				std::string filename = Application::Get().OpenFile("");
		//				if (filename != "")
		//					a.second = Texture2D::Create(filename);
		//			}
		//		}
		//		//ImGui::SameLine();
		//		//ImGui::BeginGroup();
		//		//ImGui::Checkbox("Use##AlbedoMap", &m_AlbedoInput.UseTexture);
		//		//if (ImGui::Checkbox("sRGB##AlbedoMap", &m_AlbedoInput.SRGB))
		//		//{
		//		//	if (m_AlbedoInput.TextureMap)
		//		//		m_AlbedoInput.TextureMap.reset(Hazel::Texture2D::Create(m_AlbedoInput.TextureMap->GetPath(), m_AlbedoInput.SRGB));
		//		//}
		//		//ImGui::EndGroup();
		//		//ImGui::SameLine();
		//		//ImGui::ColorEdit3("Color##Albedo", glm::value_ptr(m_AlbedoInput.Color), ImGuiColorEditFlags_NoInputs);
		//	}
		//}
	}

}
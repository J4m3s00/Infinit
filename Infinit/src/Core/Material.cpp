#include "inpch.h"
#include "Material.h"

#include "Core/Log.h"

#include "Core/ImGui/ImGuiHelper.h"
#include "Core/Application.h"

namespace Infinit {

	void Parameter::BindToShader(std::shared_ptr<Shader> shader)
	{
		m_Buffer = shader->GetUniformBuffer(m_Name);
	}

	Material::Material(std::shared_ptr<Shader> shader)
		: ShaderProgram(shader)
	{

	}

	Material::~Material()
	{
		for (Parameter* p : m_Params)
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
		for (const auto& tex : m_Textures)
		{
			uint slot = ShaderProgram->GetResourceSlot(tex.first);
			tex.second->Bind(slot);
		}
		ShaderProgram->UploadUniformBuffer();
	}

	void Material::AddTexture(const string& shaderName, std::shared_ptr<Texture2D> texture)
	{
		MaterialParameter<int>* param = new MaterialParameter<int>(shaderName);
		AddParameter(param);
		*param->Value = ShaderProgram->GetResourceSlot(shaderName);
		m_Textures[shaderName] = texture;
	}

	void Material::AddTexture(const string& shaderName, std::shared_ptr<TextureCube> texture)
	{
		MaterialParameter<int>* param = new MaterialParameter<int>(shaderName);
		AddParameter(param);
		*param->Value = ShaderProgram->GetResourceSlot(shaderName);
		m_Textures[shaderName] = texture;
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
			param->DrawImGui();
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
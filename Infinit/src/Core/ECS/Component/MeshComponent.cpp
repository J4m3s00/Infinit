#include "inpch.h"
#include "MeshComponent.h"
#include "Core/ECS/GameObject.h"

#include "graphics/Renderer.h"
#include "Core/Application.h"

#include <imgui.h>
#include <filesystem>

namespace Infinit {

	void MeshComponent::OnEvent(Event& e)
	{
		if (e.GetEventType() == EventType::AppRender)
		{
			if (MMesh && !m_Instance)
			{
				m_Instance = new MeshInstance(MMesh);
				//m_Instance->UsedMaterial = Material::DefaultMaterial;
			}
			if (m_Instance)
			{
				Infinit::Renderer::Draw(m_Instance, m_GameObject->GetWorldTransform().GetTransformMatrix());
			}
		}
	}

	void MeshComponent::DrawImGui()
	{
		if (m_Instance)
		{
			if (m_Instance->UsedMaterial)
			{
				ImGui::Begin("Material##MaterialWindow");
				m_Instance->UsedMaterial->DrawImGui();
				ImGui::End();
			}
		}

		if (ImGui::CollapsingHeader(GetTypeName().c_str()))
		{
			if (m_Instance)
			{
				m_Instance->DrawImGui();
				ImGui::SameLine();
			}
			ImGui::Text("Mesh:");
			ImGui::SameLine();
			if (ImGui::Button("Load##Mesh"))
			{
				std::string filename = Application::Get().OpenFile(IN_FILE_FILTER_Mesh);
				
				Application::Get().GetResource(filename, [this](std::shared_ptr<Resource> mesh) {this->MMesh = std::dynamic_pointer_cast<Mesh>(mesh); });
			}
			if (m_Instance)
			{
				if (m_Instance->UsedMaterial)
				{
					ImGui::Text(m_Instance->UsedMaterial->GetName().c_str());
				}
				else
				{
					ImGui::Text("Material:");
					ImGui::SameLine();
					if (ImGui::Button("Load##Material"))
					{
						string filePath = Application::Get().OpenFile(IN_FILE_FILTER_Material);

						Application::Get().GetResource(filePath, [this](std::shared_ptr < Resource> material) {this->m_Instance->UsedMaterial = std::dynamic_pointer_cast<Material>(material); });
					}
				}
			}
		}
	}

}
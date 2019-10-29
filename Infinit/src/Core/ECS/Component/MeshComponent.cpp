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
				
				if (m_Instance)
				{
					delete m_Instance;
					m_Instance = nullptr;
				}
				m_Instance = new MeshInstance(Application::Get().GetResource<Mesh>(filename));
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

						m_Instance->UsedMaterial = Application::Get().GetResource<Material>(filePath);
					}
				}
			}
		}
	}

}
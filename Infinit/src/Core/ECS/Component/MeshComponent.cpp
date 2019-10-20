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
			if (Instance)
				Renderer::Draw(Instance, m_GameObject->GetWorldTransform().GetTransformMatrix());
		}
	}

	void MeshComponent::DrawImGui()
	{
		if (ImGui::CollapsingHeader(GetTypeName().c_str()))
		{
			if (Instance)
			{
				Instance->DrawImGui();
				ImGui::SameLine();
			}
			ImGui::Text("Mesh:");
			ImGui::SameLine();
			if (ImGui::Button("Load##Mesh"))
			{
				std::string filename = Application::Get().OpenFile(IN_FILE_FILTER_Mesh);
				
				std::shared_ptr<Mesh> mesh = std::dynamic_pointer_cast<Mesh>(Application::Get().GetResource(filename));
				if (!mesh) return;
				Instance = new MeshInstance(mesh);
				//Instance->UsedMaterial = std::dynamic_pointer_cast<Material>(Application::Get().GetResource("res/material/TestMaterial.lua"));

				IN_CORE_TRACE("FileName: {0}, {1}", filename, mesh->GetFilePath());
			}
			if (Instance)
			{
				if (Instance->UsedMaterial)
				{
					ImGui::Text(Instance->UsedMaterial->GetName().c_str());

					ImGui::Begin("Material##MaterialWindow");
					Instance->UsedMaterial->DrawImGui();
					ImGui::End();
				}
				else
				{
					ImGui::Text("Material:");
					ImGui::SameLine();
					if (ImGui::Button("Load##Material"))
					{
						string filePath = Application::Get().OpenFile(IN_FILE_FILTER_Material);

						Instance->UsedMaterial = std::dynamic_pointer_cast<Material>(Application::Get().GetResource(filePath));
					}
				}
			}
		}
	}

}
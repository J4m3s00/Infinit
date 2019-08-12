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
			if (ImGui::Button("Load"))
			{
				std::string filename = Application::Get().OpenFile("");
				
				std::shared_ptr<Mesh> mesh = std::dynamic_pointer_cast<Mesh>(Application::Get().GetResource(filename));
				Instance = new MeshInstance(mesh);
				Instance->Material = std::dynamic_pointer_cast<Material>(Application::Get().GetResource("res/material/TestMaterial.lua"));

				IN_CORE_TRACE("FileName: {0}, {1}", filename, mesh->GetFilePath());
			}
			if (Instance)
			{
				if (Instance->Material)
				{
					ImGui::Begin("Material");
					Instance->Material->DrawImGui();
					ImGui::End();
				}
			}
		}
	}

}
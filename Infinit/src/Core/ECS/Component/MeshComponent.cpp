#include "inpch.h"
#include "MeshComponent.h"
#include "Core/ECS/GameObject.h"

#include "graphics/Renderer.h"
#include "Core/Application.h"

#include <imgui.h>

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
				Mesh* mesh = new Mesh(filename);
				Instance = new MeshInstance(mesh);
			}
		}
	}

}
#include "inpch.h"

namespace Infinit {

	void MeshComponent::OnEvent(Event& e)
	{
		if (e.GetEventType() == EventType::AppRender)
		{
			if (m_Instance)
			{
				Infinit::Renderer::Draw(m_Instance, UsedMaterial, m_GameObject->GetWorldTransform().GetTransformMatrix());
			}
		}
	}

	void MeshComponent::DrawImGui()
	{
		if (UsedMaterial)
		{
			ImGui::Begin("Material##MaterialWindow");
			UsedMaterial->DrawImGui();
			ImGui::End();
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
			ImGui::Button("", ImVec2(64, 64));
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("RESOURCE_NODE"))
				{
					ResourceNode* node = (ResourceNode*)payload->Data;
					if (node->GetType() == ResourceNode::Type::MESH)
					{
						if (m_Instance) delete m_Instance;
						m_Instance = new MeshInstance(node->GetResource<Mesh>());
					}
				}
				ImGui::EndDragDropTarget();
			}

			if (UsedMaterial)
			{
				ImGui::Text(UsedMaterial->Instance->GetName().c_str());
			}

			ImGui::Text("Material:");
			ImGui::SameLine();
			ImGui::Button("##Material", ImVec2(64, 64));
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("RESOURCE_NODE"))
				{
					ResourceNode* node = (ResourceNode*)payload->Data;
					if (node->GetType() == ResourceNode::Type::MATERIAL)
					{
						if (UsedMaterial) delete UsedMaterial;
						UsedMaterial = new MaterialInstance(node->GetResource<Material>());
					}
				}
				ImGui::EndDragDropTarget();
			}
		}
	}

}
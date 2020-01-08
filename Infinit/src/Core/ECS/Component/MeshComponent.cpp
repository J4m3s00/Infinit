#include "inpch.h"

namespace Infinit {

	//TODO: Delete instances
	void MeshComponent::OnEvent(Event& e)
	{
		if (e.GetEventType() == EventType::AppRender)
		{
			if (m_Mesh)
			{
				Infinit::Renderer::Draw(m_Mesh, UsedMaterial, m_GameObject->GetWorldTransform().GetTransformMatrix());
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
			if (m_Mesh)
			{
				ImGui::Text(m_Mesh->GetName().c_str());
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
					if (node->GetResource<Resource>() && node->GetResource<Resource>()->GetType() == Resource::Type::MESH)
					{
						m_Mesh = node->GetResource<Mesh>();
					}
				}
				ImGui::EndDragDropTarget();
			}

			if (UsedMaterial)
			{
				ImGui::Text(UsedMaterial->Instance.lock()->GetName().c_str());
			}

			ImGui::Text("Material:");
			ImGui::SameLine();
			if (ImGui::Button("Create##Material", ImVec2(64, 64)))
			{
				ImGui::OpenPopup("Create Material##NewMatrial");
			}
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("RESOURCE_NODE"))
				{
					ResourceNode* node = (ResourceNode*)payload->Data;
					if (node->GetResource<Resource>() && node->GetResource<Resource>()->GetType() == Resource::Type::MATERIAL)
					{
						if (UsedMaterial) delete UsedMaterial;
						UsedMaterial = new MaterialInstance(node->GetResource<Material>());
					}
				}
				ImGui::EndDragDropTarget();
			}
			bool open = true;
			if (ImGui::BeginPopupModal("Create Material##NewMatrial", &open, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize))
			{
				static string materialName;
				ImGui::InputText("Name", &materialName[0], 512);
				if (ImGui::Button("Ok"))
				{
					std::shared_ptr<Material> material = std::make_shared<Material>(materialName);
					Application::Get().GetResourceLoader().AddNotSavedResource(material);
					UsedMaterial = new MaterialInstance(material);

					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}
		}
	}

}
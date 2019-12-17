#include "inpch.h"
#include <imgui_internal.h>

namespace Infinit {

	Layer::Layer(const string& name)
		: m_Name(name), m_SelectedGameObject(0)
	{}

	Layer::~Layer()
	{
		for (GameObject* go : m_GameObjects)
			delete go;
	}

	void Layer::Attach()
	{
		OnAttach();
	}

	void Layer::Detach()
	{
		OnDetach();
	}

	void Layer::AddGameObject(GameObject* go)
	{
		go->OnInit();
		m_GameObjects.push_back(go);
	}

	void Layer::DrawImGui()
	{
		ImGui::CollapsingHeader(m_Name.c_str(), ImGuiTreeNodeFlags_DefaultOpen); 
		//Right click
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Add GameObject"))
			{
				AddGameObject(new GameObject("Game Object" + std::to_string(GameObjectID)));
			}
			ImGui::EndPopup();
		}

		for (GameObject* go : m_GameObjects)
		{
			DrawGameObjectImGui(go);
		}

		ImGui::Begin("GameObject##GameObject");
		if (m_SelectedGameObject)
			m_SelectedGameObject->DrawImGui();
		ImGui::End();
	}

	void Layer::OnEvent(Event& e)
	{
		for (GameObject* go : m_GameObjects)
		{
			go->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	void Layer::DrawGameObjectImGui(GameObject* go)
	{
		bool showNode = ImGui::TreeNode((go->GetName() + "##" + std::to_string(go->GetObjectID())).c_str());
		//Left click
		if (ImGui::IsItemClicked())
		{
			m_SelectedGameObject = go;
		}
		//Right click
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Add GameObject"))
			{
				go->AddChild(new GameObject("Game Object" + std::to_string(GameObjectID)));
			}
			if (ImGui::BeginMenu("Add Component"))
			{
				if (ImGui::MenuItem("Mesh Component"))
				{
					go->AddComponent<MeshComponent>();
				}
				ImGui::EndMenu();
			}
			ImGui::EndPopup();
		}
		if (showNode)
		{
			for (GameObject* child : go->GetChilds())
			{
				DrawGameObjectImGui(child);
			}
			ImGui::TreePop();
		}
	}

}
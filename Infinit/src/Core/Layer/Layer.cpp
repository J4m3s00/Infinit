#include "inpch.h"
#include "Layer.h"

#include "Core/ECS/GameObject.h"
#include <imgui.h>

namespace Infinit {

	Layer::Layer(const string& name)
		: m_Name(name)
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

	void Layer::Update()
	{
		for (GameObject* go : m_GameObjects)
			go->OnUpdate();
		OnUpdate();
	}

	void Layer::Render()
	{
		for (GameObject* go : m_GameObjects)
			go->OnRender();

		OnRender();
	}

	void Layer::ImGuiRender()
	{
		OnImGuiRender();
	}

	void Layer::AddGameObject(GameObject* go)
	{
		go->OnInit();
		m_GameObjects.push_back(go);
	}

	void Layer::DrawImGui()
	{
		for (GameObject* go : m_GameObjects)
		{
			if (ImGui::CollapsingHeader(go->GetName().c_str()))
			{
				for (GameObject* child : go->GetChilds())
				{
					DrawGameObjectImGui(child);
				}
			}
		}
	}

	void Layer::DrawGameObjectImGui(GameObject* go)
	{
		if (ImGui::TreeNode(go->GetName().c_str()))
		{
			for (GameObject* child : go->GetChilds())
			{
				DrawGameObjectImGui(child);
			}
			ImGui::TreePop();
		}
	}

}
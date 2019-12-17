#include "inpch.h"

namespace Infinit {

	GameObject::GameObject(const string& name, const Transform& transform)
		: m_Name(name), m_LocalTransform(transform)
	{
		m_ObjectID = GameObjectID++;
	}

	GameObject::~GameObject()
	{}

	void GameObject::OnEvent(Event& e)
	{
		//Components first!
		for (Component* c : m_Components)
		{
			c->_process_event(e);
			if (e.Handled)
				break;
		}

		if (e.GetEventType() == EventType::AppRender)
		{
			m_Transform = m_LocalTransform + ((AppRenderEvent&)e).GetTransform();
			for (GameObject* go : m_Childs)
				go->OnEvent((Event&) AppRenderEvent(m_Transform));
		}
		else
		{
			for (GameObject* go : m_Childs)
			{
				go->OnEvent(e);
				if (e.Handled)
					break;
			}
		}
	}

	void GameObject::OnInit()
	{

	}

	void GameObject::OnUpdate()
	{

	}

	void GameObject::OnRender(const Transform& parentTransform)
	{
		m_Transform = m_LocalTransform + parentTransform;
//		if (StaticMesh) Renderer::Draw(StaticMesh.get(), m_Transform.GetTransformMatrix());
		for (GameObject* go : m_Childs)
			go->OnRender(m_Transform);
	}

	void GameObject::DrawImGui()
	{
		m_LocalTransform.DrawImGui();

		ImGui::Begin("Components##ComponentView");
		for (Component* c : m_Components)
		{
			c->DrawImGui();
		}
		ImGui::End();
	}

}
#include "inpch.h"
#include "GameObject.h"

#include <imgui.h>

//TEMP
#include "graphics/Renderer.h"

namespace Infinit {

	GameObject::GameObject(const string& name, const Transform& transform)
		: m_Name(name), m_LocalTransform(transform)
	{
	}

	GameObject::~GameObject()
	{}

	void GameObject::OnInit()
	{

	}

	void GameObject::OnUpdate()
	{

	}

	void GameObject::OnRender(const Transform& parentTransform)
	{
		m_Transform = m_LocalTransform + parentTransform;
		if (StaticMesh) Renderer::Draw(StaticMesh.get(), m_Transform.GetTransformMatrix());
		for (GameObject* go : m_Childs)
			go->OnRender(m_Transform);
	}

	void GameObject::DrawImGui()
	{
		m_LocalTransform.DrawImGui();
	}

}
#pragma once

#include "inpch.h"
#include "Core/Core.h"
#include "Core/Transform.h"
//TEMP
#include "graphics/Mesh.h"

namespace Infinit {

	class GameObject
	{
	public:
		GameObject(const string& name, const Transform& tranform = Transform());
		~GameObject();

		const string& GetName() const { return m_Name; }

		void OnInit();
		void OnUpdate();
		void OnRender(const Transform& parentTransform = Transform());

		Transform& GetLocalTransform() { return m_LocalTransform; }
		const Transform& GetWorldTransform() const { return m_Transform; }

		std::vector<GameObject*> GetChilds() { return m_Childs; }
		GameObject* AddChild(GameObject* child) { m_Childs.push_back(child); return this; }

		void DrawImGui();
	private:
		string m_Name;
		std::vector<GameObject*> m_Childs;
		GameObject* m_Parent;
		Transform m_LocalTransform; //Local Transform
		Transform m_Transform;		//World Transform
	public:
		std::shared_ptr<MeshInstance> StaticMesh;
	};

}
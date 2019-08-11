#pragma once

#include "inpch.h"
#include "Core/Core.h"
#include "Core/Transform.h"
#include "Core/Log.h"

#include "Core/ECS/Component/Component.h"

namespace Infinit {

	static uint GameObjectID = 0;

	class GameObject
	{
	public:
		GameObject(const string& name, const Transform& tranform = Transform());
		~GameObject();

		const string& GetName() const { return m_Name; }

		void OnEvent(Event& e);

		void OnInit();
		void OnUpdate();
		void OnRender(const Transform& parentTransform = Transform());

		template <typename T>
		T* AddComponent()
		{
			T* result = new T();
			result->m_GameObject = this;
			m_Components.push_back(result);
			return result;
		}

		template <typename T>
		T* GetComponent()
		{
			for (Component* c : m_Components)
				if (c->GetTypeName() == T::GetTypeNameStatic())
					return c;
			IN_CORE_WARN("Could not find Component {0}!", T::GetTypeNameStatic());
			return nullptr;
		}

		Transform& GetLocalTransform() { return m_LocalTransform; }
		const Transform& GetWorldTransform() const { return m_Transform; }

		std::vector<GameObject*> GetChilds() { return m_Childs; }
		GameObject* AddChild(GameObject* child) { m_Childs.push_back(child); return this; }

		const uint GetObjectID() const { return m_ObjectID; }
		void DrawImGui();
	private:
		uint m_ObjectID;
		string m_Name;
		std::vector<GameObject*> m_Childs;
		std::vector<Component*> m_Components;
		GameObject* m_Parent;
		Transform m_LocalTransform; //Local Transform
		Transform m_Transform;		//World Transform
	};

}
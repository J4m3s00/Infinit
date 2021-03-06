#pragma once

namespace Infinit {
	class Scene;
	class GameObject;

	class INFINIT_API Layer
	{
	public:
		Layer(const string& name);
		virtual ~Layer();

		void Attach();
		void Detach();

		void DrawImGui();

		virtual void OnEvent(Event& e);
		//Maybe layers should not be extendable
	protected:
		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate() {}
		virtual void OnRender() {}
		virtual void OnImGuiRender() {}

		const string& GetName() const { return m_Name; }
		
		void AddGameObject(GameObject* gameObject);
	private:
		void DrawGameObjectImGui(GameObject* gameObject);
	public:
		Scene* Scene;
	protected:
		string m_Name;
		std::vector<GameObject*> m_GameObjects;
		GameObject* m_SelectedGameObject;
	};

}
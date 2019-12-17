#pragma once

namespace Infinit {

	class MeshComponent : public Component
	{
		IN_COMPONENT(MeshComponent, Component)
	public:
		MeshComponent() : m_Instance(nullptr), UsedMaterial(nullptr){}
		virtual ~MeshComponent() { if (m_Instance) delete m_Instance; }

		virtual void OnEvent(Event& e);
		virtual void DrawImGui();
	public:
		MeshInstance* m_Instance;
		MaterialInstance* UsedMaterial;
	private:
	};

}
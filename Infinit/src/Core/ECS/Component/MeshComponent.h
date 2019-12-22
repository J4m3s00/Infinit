#pragma once

namespace Infinit {

	class MeshComponent : public Component
	{
		IN_COMPONENT(MeshComponent, Component)
	public:
		MeshComponent() : m_Mesh(nullptr), UsedMaterial(nullptr){}
		virtual ~MeshComponent() {  }

		virtual void OnEvent(Event& e);
		virtual void DrawImGui();
	public:
		std::shared_ptr<Mesh> m_Mesh;
		MaterialInstance* UsedMaterial;
	private:
	};

}
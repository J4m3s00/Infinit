#pragma once

#include "Component.h"
#include "graphics/Mesh.h"

namespace Infinit {

	class MeshComponent : public Component
	{
		IN_COMPONENT(MeshComponent, Component)
	public:
		MeshComponent() : MMesh(nullptr), m_Instance(nullptr){}

		virtual void OnEvent(Event& e);
		virtual void DrawImGui();
	public:
		std::shared_ptr<Mesh> MMesh;
		MeshInstance* m_Instance;
	private:
	};

}
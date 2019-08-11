#pragma once

#include "Component.h"
#include "graphics/Mesh.h"

namespace Infinit {

	class MeshComponent : public Component
	{
		IN_COMPONENT(MeshComponent, Component)
	public:
		MeshComponent() : Instance(nullptr){}

		virtual void OnEvent(Event& e);
		virtual void DrawImGui();
	public:
		MeshInstance* Instance;
	};

}
#pragma once

#include "Core/Core.h"
#include "Events/Events.h"

#define IN_COMPONENT(baseClass, inheritance) public:\
	virtual string GetTypeName() const { return string(#baseClass); };\
	static string GetTypeNameStatic() { return string(#baseClass); }\
	virtual void _process_event(Event& e){\
		inheritance::_process_event(e);\
		OnEvent(e);\
	}

namespace Infinit {

	class Component
	{
		friend class GameObject;
	public:
		Component() {}
		virtual string GetTypeName() const { return "Component"; }

		virtual void _process_event(Event& e)
		{
			OnEvent(e);
		}
	protected:
		void OnEvent(Event& e) {}
		virtual void DrawImGui();
	protected:
		GameObject* m_GameObject;
	};

}
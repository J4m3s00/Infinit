#pragma once

#include "Core/Core.h"
#include "Events/Event.h"

namespace Infinit {

	class INFINIT_API Layer
	{
	public:
		Layer(const string& name);
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate() {}
		virtual void OnRender() {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& e) {}

		const string& GetName() const { return m_Name; }
	private:
		string m_Name;
	};

}
#pragma once

#include "Core.h"

namespace Infinit {

	class INFINIT_API Application 
	{
	private:
		bool m_Running;
	protected:
		Application(const std::string& name);
	public:
		virtual ~Application();

		void OnTick() {}
		void OnUpdate() {}
		void OnRender() {}

		void Run();
	};

	Application* CreateApplication();

}
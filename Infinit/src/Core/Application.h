#pragma once

#include <Infinit.h>
#include <string>

namespace Infinit {

	class INFINIT_API Application 
	{
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
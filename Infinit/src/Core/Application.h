#pragma once

#include "Core.h"
#include "graphics/Window.h"
#include "Events/Event.h"
#include "Events/WindowEvent.h"
#include "Layer/LayerStack.h"
#include "ECS/Scene.h"
#include "graphics/RendererAPI.h"

namespace Infinit {

	class Shader;
	class VertexArray;
	class VertexBuffer;
	class IndexBuffer;

	class INFINIT_API Application 
	{
	protected:
		Application(const std::string& name, RendererAPI::Type renderer);
	public:
		virtual ~Application();

		void OnEvent(Event& event);

		void SetActiveScene(Scene* scene);

		void Run();

		inline Window& GetWindow() { return *m_Window; }
		inline static Application& Get() { return *s_Instance; }
	private:
		bool Init();
		bool OnWindowClose(WindowCloseEvent& e);

		void ImGuiInit();
		void ImGuiDestroy();
		void ImGuiBegin();
		void ImGuiEnd();
	private:
		Scene* m_ActiveScene;

		bool m_Running;
		string m_Name;
		std::unique_ptr<Window> m_Window;
	private:
		static Application* s_Instance;
	};

	Application* CreateApplication();

}
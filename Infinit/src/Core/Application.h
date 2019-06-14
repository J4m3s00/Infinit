#pragma once

#include "Core.h"
#include "graphics/Window.h"
#include "Events/Event.h"
#include "Events/WindowEvent.h"
#include "Layer/LayerStack.h"
#include "Layer/ImGuiLayer.h"

namespace Infinit {

	class Shader;
	class VertexArray;
	class VertexBuffer;
	class IndexBuffer;

	class INFINIT_API Application 
	{
	protected:
		Application(const std::string& name);
	public:
		virtual ~Application();

		void OnEvent(Event& event);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);
		void PopLayer(Layer* layer);
		void PopOverlay(Layer* layer);

		void Run();

		inline Window& GetWindow() { return *m_Window; }
		inline static Application& Get() { return *s_Instance; }
	private:
		bool Init();
		bool OnWindowClose(WindowCloseEvent& e);
	private:
		LayerStack m_LayerStack;
		ImGuiLayer* m_ImGuiLayer;

		bool m_Running;
		string m_Name;
		std::unique_ptr<Window> m_Window;

		std::shared_ptr<Shader> m_Shader;
		std::unique_ptr<VertexArray> m_VAO;
		uint m_VAOID;
	private:
		static Application* s_Instance;
	};

	Application* CreateApplication();

}
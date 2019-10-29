#pragma once

#include "Core.h"
#include "Resource.h"
#include "graphics/Window.h"
#include "Events/Event.h"
#include "Events/WindowEvent.h"
#include "Layer/LayerStack.h"
#include "ECS/Scene.h"
#include "graphics/RendererAPI.h"
#include "ResourceLoader.h"

namespace Infinit {

	class Shader;
	class VertexArray;
	class VertexBuffer;
	class IndexBuffer;

	class Application 
	{
	protected:
		Application(const std::string& name, RendererAPI::Type renderer);
	public:
		virtual ~Application();

		void OnEvent(Event& event);

		void SetActiveScene(Scene* scene);

		void Run();

		const string& GetApplicationPath() const { return m_ResourcePath; }

		string OpenFile(const LPCSTR& filter) const;

		template <typename T>
		std::shared_ptr<T> GetResource(const string& filePath)
		{
			return std::dynamic_pointer_cast<T>(m_ResourceLoader.GetResource(filePath));
		}

		inline Window& GetWindow() { return *m_Window; }
		inline static Application& Get() { return *s_Instance; }
	public:
		bool Init();
		void LoadAllResources(const string& folder);
		//void SaveResourceInCache(const string& relativPath, const string& absolutePath);
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
		string m_ResourcePath;
	private:
		static Application* s_Instance;
		std::vector<std::future<void>> m_Futures;
		ResourceLoader m_ResourceLoader;
	public:
		static int LaunchArgumentCount;
		static char** LaunchArguments;
	};

	Application* CreateApplication();

}

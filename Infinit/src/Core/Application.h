#pragma once

#include "Core.h"
#include "Resource.h"
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

		string OpenFile(const LPCSTR& filter) const;
		std::shared_ptr<Resource> GetResource(const string& filePath);

		inline Window& GetWindow() { return *m_Window; }
		inline static Application& Get() { return *s_Instance; }
	public:
		bool Init();
		void LoadAllResources(const string& folder);
		void SaveResourceInCache(const string& relativPath, const string& absolutePath);
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
		std::unordered_map<string, std::shared_ptr<Resource>> m_ResourceCache;
		string m_ResourcePath;
	private:
		static Application* s_Instance;
	public:
		static int LaunchArgumentCount;
		static char** LaunchArguments;
	};

	Application* CreateApplication();

}

#include <inpch.h>
#include "Application.h"
#include "Log.h"
#include "Layer/Layer.h"

#include <glad/glad.h>

#include <graphics/Shader.h>
#include <graphics/VertexArray.h>
#include <graphics/Buffer.h>

#include <glm/glm.hpp>
#include <graphics/Renderer.h>
#include "Util/StringUtil.h"

#ifdef IN_PLATFORM_WINDOWS
#include <filesystem>
#endif

namespace Infinit {

	struct Vertex
	{
		glm::vec3 position;
		uint color;
	};

	Application* Application::s_Instance = nullptr;

	Application::Application(const std::string& name, RendererAPI::Type renderer) 
		: m_Name(name), m_LayerStack()
	{
		RendererAPI::Renderer = renderer;
		s_Instance = this;
		m_Running = Init();
	}

	Application::~Application()
	{

	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer)
	{
		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	void Application::PopLayer(Layer* layer)
	{
		m_LayerStack.PopLayer(layer);
		layer->OnDetach();
	}

	void Application::PopOverlay(Layer* layer)
	{
		m_LayerStack.PopOverlay(layer);
		layer->OnDetach();
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher disp(e);
		disp.Dispatch<WindowCloseEvent>(IN_BIND_EVENT_FN(Application::OnWindowClose));

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
		{
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	void Application::Run()
	{
		while (m_Running)
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();

			for (Layer* layer : m_LayerStack)
				layer->OnRender();
			

			ImGuiLayer::Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();
			ImGuiLayer::End();

			m_Window->Update();
		}
	}

	bool Application::Init()
	{
		m_Window.reset(new Window({ m_Name, 1270, 720 }));
		if (!m_Window) return false;
		m_Window->SetEventCallback(IN_BIND_EVENT_FN(Application::OnEvent));

		RendererAPI::Init();
		Renderer::Init();

		LoadAllResources("res/");

		//m_ImGuiLayer = new ImGuiLayer();
		//PushLayer(m_ImGuiLayer);
		
		return true;
	}

	void Application::LoadAllResources(const string& folder)
	{
		for (const auto& entry : std::filesystem::directory_iterator(folder))
		{
			if (entry.is_directory())
				LoadAllResources(entry.path().u8string());
			else
			{
				string filePath = entry.path().u8string();
				SaveResourceInCache(filePath);
			}
		}
	}

	void Application::SaveResourceInCache(const string& filePath)
	{
		string fileEnding = filePath.substr(filePath.find_last_of(".") + 1, filePath.size());
		//Textures
		if (fileEnding == "png" || fileEnding == "tga")
		{
			m_ResourceCache[filePath] = Texture2D::Create(filePath);
		}
		//Meshes
		else if (fileEnding == "fbx")
		{
			m_ResourceCache[filePath] = std::shared_ptr<Mesh>(new Mesh(filePath));
		}
		//Shaders
		else if (fileEnding == "shader")
		{
			m_ResourceCache[filePath] = Shader::Create(filePath);
		}
	}

	std::shared_ptr<Resource> Application::GetResource(const string& filePath)
	{
		std::unordered_map<string, std::shared_ptr<Resource>>::iterator it = m_ResourceCache.find(filePath);
		if (it != m_ResourceCache.end())
		{
			return it->second;
		}
		IN_CORE_WARN("Could not find Resource: \"{0}\"", filePath);
		return nullptr;
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}
}
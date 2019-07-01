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

//Bring imgui for all platforms
#include <GLFW/glfw3.h>

#include <imgui.h>
#include "examples/imgui_impl_glfw.h"
#include "examples/imgui_impl_opengl3.h"

namespace Infinit {

	struct Vertex
	{
		glm::vec3 position;
		uint color;
	};

	Application* Application::s_Instance = nullptr;

	Application::Application(const std::string& name, RendererAPI::Type renderer) 
		: m_Name(name)
	{
		RendererAPI::Renderer = renderer;
		s_Instance = this;
		m_Running = Init();
	}

	Application::~Application()
	{

	}

	void Application::SetActiveScene(Scene* scene)
	{
		m_ActiveScene->Detach();
		m_ActiveScene = scene;
		m_ActiveScene->Attach();
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher disp(e);
		disp.Dispatch<WindowCloseEvent>(IN_BIND_EVENT_FN(Application::OnWindowClose));

		m_ActiveScene->OnEvent(e);
	}

	void Application::Run()
	{
		while (m_Running)
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			IN_CORE_ASSERT(m_ActiveScene, "No ActiveScene Set!"); //Forgot to call Application::SetActiveScene();

			m_ActiveScene->Update();

			m_ActiveScene->Render();
			

			ImGuiBegin();
			m_ActiveScene->ImGuiRender();
			ImGuiEnd();

			m_Window->Update();
		}

		ImGuiDestroy();
	}

	bool Application::Init()
	{
		m_Window.reset(new Window({ m_Name, 1270, 720 }));
		if (!m_Window) return false;
		m_Window->SetEventCallback(IN_BIND_EVENT_FN(Application::OnEvent));

		RendererAPI::Init();
		Renderer::Init();
		ImGuiInit();

		return true;
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

	void Application::ImGuiInit()
	{
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGuiContext* context = ImGui::CreateContext();
		ImGui::SetCurrentContext(context);
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;

		// Setup Dear ImGui style
		ImGui::StyleColorsLight();
		//ImGui::StyleColorsClassic();

		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		// Setup Platform/Renderer bindings
		ImGui_ImplGlfw_InitForOpenGL(m_Window->GetNativeWindow(), true);
		ImGui_ImplOpenGL3_Init("#version 410");
	}

	void Application::ImGuiDestroy()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void Application::ImGuiBegin()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void Application::ImGuiEnd()
	{
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();
		io.DisplaySize = ImVec2(app.GetWindow().GetWidth(), app.GetWindow().GetHeight());

		// Rendering
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}
}
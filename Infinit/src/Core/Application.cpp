#include "inpch.h"

#define IMGUI_IMPL_OPENGL_LOADER_GLAD
#include "examples/imgui_impl_glfw.h"
#include "examples/imgui_impl_opengl3.h"

namespace Infinit {

	struct Vertex
	{
		glm::vec3 position;
		uint color;
	};

	Application* Application::s_Instance = nullptr;
	int Application::LaunchArgumentCount = 0;
	char** Application::LaunchArguments = nullptr;

	Application::Application(const std::string& name, RendererAPI::Type renderer) 
		: m_Name(name)
	{
		std::filesystem::path path("./");
		m_ResourcePath = std::filesystem::absolute(path).u8string();
		std::replace(m_ResourcePath.begin(), m_ResourcePath.end(), '\\', '/');

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
		if (!m_ActiveScene->ActiveCamera)
			m_ActiveScene->ActiveCamera = new Infinit::Camera(glm::perspective(65.0f, 16.0f / 9.0f, 0.0001f, 10000.0f));

		if (m_ActiveScene->LightMap.size() <= 0)
			m_ActiveScene->LightMap.push_back({ { 0.2f, 0.34f, 0.5f }, { 1.0f, 1.0f, 1.0f } });
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
			IN_CORE_ASSERT(m_ActiveScene, "No ActiveScene Set!"); //Forgot to call Application::SetActiveScene();

			//OnEvent(AppUpdateEvent(0.0f));
			m_ActiveScene->Update();

			//OnEvent(AppRenderEvent());
			m_ActiveScene->Render();
			Renderer::Get()->WaitAndRender();

			ImGuiBegin();
			//OnEvent(AppImGuiRenderEvent());
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

		std::filesystem::path resourcePath("res/");
		if (!std::filesystem::exists(resourcePath))
		{
			std::filesystem::create_directory(resourcePath);
		}

		LoadAllResources(resourcePath);
		m_ResourceLoader.LoadCompleteResourceTree();
		//SaveResourceInCache("res/cerberus.fbx");

		//m_ImGuiLayer = new ImGuiLayer();
		//PushLayer(m_ImGuiLayer);
		
		return true;
	}

	//Not working right now!
	void Application::LoadAllResources(const std::filesystem::path& folder)
	{
		for (const auto& entry : std::filesystem::directory_iterator(folder))
		{
			string filePath = entry.path().u8string();
			std::replace(filePath.begin(), filePath.end(), '\\', '/');

			m_ResourceLoader.AddResourceToLoad(filePath);
			if (entry.is_directory())
			{
				LoadAllResources(entry.path().u8string());
			}
		}
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
		ImGui::StyleColorsDark();
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

	string Application::OpenFile(const LPCSTR& filter) const
	{
		OPENFILENAMEA ofn;       // common dialog box structure
		CHAR szFile[260] = { 0 };       // if using TCHAR macros

		// Initialize OPENFILENAME
		ZeroMemory(&ofn, sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)m_Window->GetNativeWindow());
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrFilter = filter;
		ofn.nFilterIndex = 1;
		ofn.lpstrFileTitle = NULL;
		ofn.nMaxFileTitle = 0;
		ofn.lpstrInitialDir = NULL;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

		if (GetOpenFileNameA(&ofn) == TRUE)
		{
			string result = ofn.lpstrFile;
			std::replace(result.begin(), result.end(), '\\', '/');

			result = result.substr(m_ResourcePath.size(), result.size() - m_ResourcePath.size());

			return result;
		}
		return std::string();
	}

	void Application::DrawResourceLoaderImGui()
	{
		m_ResourceLoader.ImGuiDraw();
	}
}

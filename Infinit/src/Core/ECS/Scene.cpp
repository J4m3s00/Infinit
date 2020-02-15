#include "inpch.h"

namespace Infinit {

	Scene::Scene(const string& name)
		: m_Name(name), ActiveCamera(NULL)
	{
		m_RenderBuffer = FrameBuffer::Create(Application::Get().GetWindow().GetWidth(), Application::Get().GetWindow().GetHeight(), FramebufferFormat::RGBA16F);
	}

	Scene::~Scene()
	{
		delete m_RenderBuffer;
	}

	void Scene::PushLayer(Layer* layer)
	{
		layer->Scene = this;
		m_LayerStack.PushLayer(layer);
	}

	void Scene::PushOverlay(Layer* layer)
	{
		layer->Scene = this;
		m_LayerStack.PushOverlay(layer);
	}

	void Scene::PopLayer(Layer* layer)
	{
		layer->Detach();
		layer->Scene = nullptr;
		m_LayerStack.PopLayer(layer);
	}

	void Scene::PopOverlay(Layer* layer)
	{
		layer->Detach();
		layer->Scene = nullptr;
		m_LayerStack.PopOverlay(layer);
	}

	void Scene::OnEvent(Event& e)
	{
		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
		{
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	void Scene::Update()
	{
		if (ActiveCamera)
			ActiveCamera->Update();
		OnEvent((Event&)AppUpdateEvent(0.0f));
	}

	void Scene::Render()
	{
		m_RenderBuffer->Bind();
		Renderer::Begin(ActiveCamera, LightMap);
		OnEvent((Event&)AppRenderEvent(Transform()));
		Renderer::End();
		m_RenderBuffer->Unbind();
	}

	void Scene::ImGuiRender()
	{
		static bool p_open;

		static bool opt_fullscreen_persistant = true;
		static ImGuiDockNodeFlags opt_flags = ImGuiDockNodeFlags_None;
		bool opt_fullscreen = opt_fullscreen_persistant;

		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar;
		if (opt_fullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}


		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &p_open, window_flags);
		ImGui::PopStyleVar();


		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// Dockspace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockspace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), opt_flags);
		}

		DrawImGui();
		OnEvent((Event&)AppImGuiRenderEvent());

		ImGui::End();
	}

	void Scene::Attach()
	{
		for (Layer* layer : m_LayerStack)
			layer->Attach();
	}

	void Scene::Detach()
	{

	}

	void Scene::DrawImGui()
	{
		Application::Get().DrawResourceLoaderImGui();


		ImGui::Begin("Resource View##ResourceView");
		ImGui::End();

		ImGui::Begin("Components##ComponentView");
		ImGui::End();

		ImGui::Begin("Material##MaterialWindow");
		ImGui::End();

		ImGui::Begin("Viewport##RenderView");
		auto viewportSize = ImGui::GetContentRegionAvail();
		//m_RenderBuffer->Resize(viewportSize.x, viewportSize.y);
		ImGui::Image((void*)m_RenderBuffer->GetColorAttachment(), viewportSize, { 0, 1 }, { 1, 0 });
		ImGui::End();

		ImGui::Begin(("Scene " + m_Name).c_str());
		for (Layer* layer : m_LayerStack)
		{
			layer->DrawImGui();
		}
		ImGui::End();
		
		ImGui::Begin("Light");
		ImGui::SliderFloat3("Direction", &LightMap[0].Direction[0], -360.0f, 360.0f);
		ImGui::SliderFloat3("Radiance", &LightMap[0].Radiance[0], 0.0f, 1.0f);
		ImGui::End();
		 
	}

}
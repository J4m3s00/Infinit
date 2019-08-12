#include "inpch.h"
#include "Scene.h"
#include "Core/Layer/Layer.h"
#include "graphics/Renderer.h"
#include <imgui.h>

namespace Infinit {

	Scene::Scene(const string& name)
		: m_Name(name), ActiveCamera(NULL)
	{

	}

	Scene::~Scene()
	{
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
		OnEvent(AppUpdateEvent(0.0f));
	}

	void Scene::Render()
	{
		Renderer::Begin(ActiveCamera, LightMap);
		OnEvent(AppRenderEvent(Transform()));
		Renderer::End();
	}

	void Scene::ImGuiRender()
	{
		DrawImGui();
		OnEvent(AppImGuiRenderEvent());
	}

	void Scene::Attach()
	{
		for (Layer* layer : m_LayerStack)
			layer->Attach();
	}

	void Scene::Detach()
	{

	}

	/*void Scene::Update()
	{
		for (Layer* layer : m_LayerStack)
			layer->Update();
	}

	void Scene::Render()
	{
		Infinit::Renderer::Begin(ActiveCamera, LightMap);
		for (Layer* layer : m_LayerStack)
			layer->Render();
		Infinit::Renderer::End();
	}

	void Scene::ImGuiRender()
	{
		for (Layer* layer : m_LayerStack)
			layer->ImGuiRender();
	}*/

	void Scene::DrawImGui()
	{
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
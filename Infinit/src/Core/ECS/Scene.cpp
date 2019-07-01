#include "inpch.h"
#include "Scene.h"
#include "Core/Layer/Layer.h"
#include "graphics/Renderer.h"

namespace Infinit {

	Scene::Scene(const string& name)
		: m_Name(name)
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
		layer->OnDetach();
		layer->Scene = nullptr;
		m_LayerStack.PopLayer(layer);
	}

	void Scene::PopOverlay(Layer* layer)
	{
		layer->OnDetach();
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

	void Scene::Attach()
	{
		for (Layer* layer : m_LayerStack)
			layer->OnAttach();
	}

	void Scene::Detach()
	{

	}

	void Scene::Update()
	{
		for (Layer* layer : m_LayerStack)
			layer->OnUpdate();
	}

	void Scene::Render()
	{
		Infinit::Renderer::Begin(m_ActiveCamera, m_LightMap);
		for (Layer* layer : m_LayerStack)
			layer->OnRender();
		Infinit::Renderer::End();
	}

	void Scene::SetActiveCamera(Camera* camera)
	{
		m_ActiveCamera = camera;
	}

	void Scene::ImGuiRender()
	{
		for (Layer* layer : m_LayerStack)
			layer->OnImGuiRender();
	}

}
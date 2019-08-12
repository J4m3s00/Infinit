#include <Infinit.h>
#include <imgui.h>
#include <glm/gtc/matrix_transform.hpp>

class TestLayer : public Infinit::Layer
{
private:
	Infinit::Camera* m_Camera;
	Infinit::Light* m_Light;
public:
	TestLayer()
		: Infinit::Layer("Test Layer")
	{

	} 



	virtual void OnAttach() override
	{
		m_Camera = new Infinit::Camera(glm::perspective(65.0f, 16.0f / 9.0f, 0.0001f, 10000.0f));
		Scene->ActiveCamera = m_Camera;
		Scene->LightMap.push_back({ { 0.2f, 0.34f, 0.5f }, { 1.0f, 1.0f, 1.0f } });
		m_Light = &Scene->LightMap[0];
	}

	virtual void OnDetach() override
	{
	}

	virtual void OnUpdate() override
	{
	}

	virtual void OnRender() override
	{
	}

	virtual void OnImGuiRender() override
	{
		
	}

	virtual void OnEvent(Infinit::Event& e) override
	{
		Layer::OnEvent(e);
		if (e.GetEventType() == Infinit::EventType::AppUpdate)
		{
			m_Camera->Update();
		}
		else if (e.GetEventType() == Infinit::EventType::AppImGuiRender)
		{
			Scene->DrawImGui();
		}
	}

};

class Sandbox : public Infinit::Application
{
private:
	Infinit::Scene scene;
public:
	Sandbox()
		: Application("Hello World", Infinit::RendererAPI::Type::OpenGL), scene("Test Scene")
	{
		scene.PushLayer(new TestLayer());
		SetActiveScene(&scene);
	}
};

Infinit::Application* Infinit::CreateApplication()
{
	return new Sandbox();
}

#include <Infinit.h>
#include <imgui.h>
#include <glm/gtc/matrix_transform.hpp>

class TestLayer : public Infinit::Layer
{
private:
	std::unique_ptr<Infinit::Mesh> m_Mesh;
	std::shared_ptr<Infinit::Camera> m_Camera;
public:
	TestLayer()
		: Infinit::Layer("Test Layer")
	{

	} 



	virtual void OnAttach() override
	{
		m_Camera.reset(new Infinit::Camera(glm::perspective(65.0f, 16.0f / 9.0f, 0.0001f, 10000.0f)));

		/*m_Mesh.reset(new Infinit::Mesh({ {{-0.5f, -0.5f, 0.0f}},
											{{0.0f, 0.5f, 0.0f}},
											{{0.5f, -0.5f, 0.0f}} }, { {0, 1, 2} }));*/
		m_Mesh.reset(new Infinit::Mesh("cerberus.fbx"));
	}

	virtual void OnDetach() override
	{
	}

	virtual void OnUpdate() override
	{
		m_Camera->Update();
	}

	virtual void OnRender() override
	{
		Infinit::Renderer::Begin(m_Camera);
		m_Mesh->Render();
		Infinit::Renderer::End();
	}

	virtual void OnImGuiRender() override
	{
	}

	virtual void OnEvent(Infinit::Event& e) override
	{
	}

};

class Sandbox : public Infinit::Application
{
public:
	Sandbox()
		: Application("Hello World", Infinit::RendererAPI::Type::OpenGL)
	{
		PushLayer(new TestLayer());
	}
};

Infinit::Application* Infinit::CreateApplication()
{
	return new Sandbox();
}
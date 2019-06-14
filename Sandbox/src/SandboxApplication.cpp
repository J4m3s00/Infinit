#include <Infinit.h>
#include <imgui.h>
#include <graphics/Shader.h>
#include <graphics/VertexArray.h>
#include <graphics/Buffer.h>

class TestLayer : public Infinit::Layer
{
private:
	
public:
	TestLayer()
		: Infinit::Layer("Test Layer")
	{

	} 



	virtual void OnAttach() override
	{
		

		
	}

	virtual void OnDetach() override
	{
	}

	virtual void OnUpdate() override
	{
		
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("TestAsd");
		ImGui::Text("Hello World");
		ImGui::End();
	}

	virtual void OnEvent(Infinit::Event& e) override
	{
	}

};

class Sandbox : public Infinit::Application
{
public:
	Sandbox()
		: Application("Hello World")
	{
		PushLayer(new TestLayer());
	}
};

Infinit::Application* Infinit::CreateApplication()
{
	return new Sandbox();
}
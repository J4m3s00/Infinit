#include <Infinit.h>
#include <imgui.h>

class TetsLay : public Infinit::Layer
{
public:
	void OnUpdate() override
	{

	}
};

class Sandbox : public Infinit::Application
{
private:
	Infinit::Scene scene;
public:
	Sandbox()
		: Application("Infinit Engine", Infinit::RendererAPI::Type::OpenGL), scene("Test Scene")
	{
		scene.PushLayer(new Infinit::Layer("Test Layer"));
		SetActiveScene(&scene);
	}
};

Infinit::Application* Infinit::CreateApplication()
{
	return new Sandbox();
}
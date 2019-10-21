#include <Infinit.h>
#include <imgui.h>

class TetsLay : public Infinit::Layer
{
private:

public:
	TetsLay()
		: Infinit::Layer("FrameBuffer Layer")
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
		scene.PushLayer(new TetsLay());
		SetActiveScene(&scene);
	}
};

Infinit::Application* Infinit::CreateApplication()
{
	return new Sandbox();
}
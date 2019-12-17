#include "inpch.h"

class Sandbox : public Infinit::Application
{
public:
	Sandbox()
		: Infinit::Application("Sandbox", Infinit::RendererAPI::Type::OpenGL)
	{
		using namespace Infinit;
		Scene* scene = new Scene("Hello World");
		scene->PushLayer(new Layer("Debug Layer"));
		SetActiveScene(scene);
	}
};

Infinit::Application* Infinit::CreateApplication()
{
	return new Sandbox();
}

int main(int argc, char** argv)
{
	Infinit::Application::LaunchArgumentCount = argc;
	Infinit::Application::LaunchArguments = argv;
	Infinit::Log::Init();
	IN_CORE_TRACE("Log initialized!");

	auto app = Infinit::CreateApplication();
	app->Run();
	delete app;
	return 0;
}

#include <Infinit.h>

class Sandbox : public Infinit::Application
{
public:
	Sandbox()
		: Application("Hello World")
	{}
};

Infinit::Application* Infinit::CreateApplication()
{
	return new Sandbox();
}

int main()
{
	Infinit::Application* app = Infinit::CreateApplication();
	app->Run();
	delete app;
}
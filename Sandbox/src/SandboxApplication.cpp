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
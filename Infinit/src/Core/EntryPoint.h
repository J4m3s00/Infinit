#pragma once

#ifdef IN_PLATFORM_WINDOWS

extern Infinit::Application* Infinit::CreateApplication();

int main(int argc, char** argv)
{
	auto app = Infinit::CreateApplication();
	app->Run();
	delete app;
	return 0;
}

#endif
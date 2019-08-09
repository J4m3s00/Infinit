#pragma once

#ifdef IN_PLATFORM_WINDOWS

extern Infinit::Application* Infinit::CreateApplication();

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

#endif
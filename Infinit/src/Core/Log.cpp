#include "inpch.h"
#include <spdlog/sinks/stdout_sinks.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Infinit {


	std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
	std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

	void Log::Init()
	{
		spdlog::set_pattern("%^[%T] %s (l.%#) %n: %v%$");

		s_CoreLogger = spdlog::stdout_color_mt("INFINIT", spdlog::color_mode::always);
		s_CoreLogger->set_level(spdlog::level::trace);

		s_ClientLogger = spdlog::stdout_color_mt("APP", spdlog::color_mode::automatic);
		s_ClientLogger->set_level(spdlog::level::trace);
	}

}
#include "inpch.h"
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Infinit {


	std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
	std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

	void Log::Init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");

		s_CoreLogger = spdlog::stdout_color_mt<std::shared_ptr<spdlog::logger>>("INFINIT", spdlog::color_mode::automatic);
		s_CoreLogger->set_level(spdlog::level::trace);

		s_ClientLogger = spdlog::stdout_color_mt<std::shared_ptr<spdlog::logger>>("APP", spdlog::color_mode::automatic);
		s_ClientLogger->set_level(spdlog::level::trace);
	}

}
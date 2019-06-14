#pragma once

#include <memory>

#include "Core.h"
#include <spdlog/spdlog.h>

namespace Infinit {

	class INFINIT_API Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};

}

#define IN_CORE_TRACE(...) ::Infinit::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define IN_CORE_INFO(...) ::Infinit::Log::GetCoreLogger()->info(__VA_ARGS__)
#define IN_CORE_WARN(...) ::Infinit::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define IN_CORE_ERROR(...) ::Infinit::Log::GetCoreLogger()->error(__VA_ARGS__)


#define IN_TRACE(...) ::Infinit::Log::GetClientLogger()->trace(__VA_ARGS__)
#define IN_INFO(...) ::Infinit::Log::GetClientLogger()->info(__VA_ARGS__)
#define IN_WARN(...) ::Infinit::Log::GetClientLogger()->warn(__VA_ARGS__)
#define IN_ERROR(...) ::Infinit::Log::GetClientLogger()->error(__VA_ARGS__)
#pragma once
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

#define IN_CORE_TRACE(...) ::Infinit::Log::GetCoreLogger()->log(spdlog::source_loc{__FILE__, __LINE__, SPDLOG_FUNCTION},spdlog::level::trace ,__VA_ARGS__)
#define IN_CORE_INFO(...) ::Infinit::Log::GetCoreLogger()->log(spdlog::source_loc{__FILE__, __LINE__, SPDLOG_FUNCTION},spdlog::level::info ,__VA_ARGS__)
#define IN_CORE_WARN(...) ::Infinit::Log::GetCoreLogger()->log(spdlog::source_loc{__FILE__, __LINE__, SPDLOG_FUNCTION},spdlog::level::warn ,__VA_ARGS__)
#define IN_CORE_ERROR(...) ::Infinit::Log::GetCoreLogger()->log(spdlog::source_loc{__FILE__, __LINE__, SPDLOG_FUNCTION},spdlog::level::err ,__VA_ARGS__)



#define IN_TRACE(...) ::Infinit::Log::GetClientLogger()->log(spdlog::source_loc{__FILE__, __LINE__, SPDLOG_FUNCTION},spdlog::level::trace ,__VA_ARGS__)
#define IN_INFO(...) ::Infinit::Log::GetClientLogger()->log(spdlog::source_loc{__FILE__, __LINE__, SPDLOG_FUNCTION},spdlog::level::info ,__VA_ARGS__)
#define IN_WARN(...) ::Infinit::Log::GetClientLogger()->log(spdlog::source_loc{__FILE__, __LINE__, SPDLOG_FUNCTION},spdlog::level::warn ,__VA_ARGS__)
#define IN_ERROR(...) ::Infinit::Log::GetClientLogger()->log(spdlog::source_loc{__FILE__, __LINE__, SPDLOG_FUNCTION},spdlog::level::err ,__VA_ARGS__)

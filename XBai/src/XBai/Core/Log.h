#ifndef XBAI_CORE_LOG_H
#define XBAI_CORE_LOG_H
#pragma warning( disable: 4251 )//忽略掉“需要有 dll 接口以供“XBai::Log”的客户端使用”warning

#include "Core.h"
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#include <spdlog/sinks/stdout_color_sinks.h>
namespace XBai
{
	class XB_API Log
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

//Core 宏
#define XB_CORE_TRACE(...)		::XBai::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define XB_CORE_INFO(...)		::XBai::Log::GetCoreLogger()->info(__VA_ARGS__)
#define XB_CORE_WARN(...)		::XBai::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define XB_CORE_ERROR(...)		::XBai::Log::GetCoreLogger()->error(__VA_ARGS__)
#define XB_CORE_CRITICAL(...)	::XBai::Log::GetCoreLogger()->critical(__VA_ARGS__)

//Client 宏
#define XB_TRACE(...)		::XBai::Log::GetClientLogger()->trace(__VA_ARGS__)
#define XB_INFO(...)		::XBai::Log::GetClientLogger()->info(__VA_ARGS__)
#define XB_WARN(...)		::XBai::Log::GetClientLogger()->warn(__VA_ARGS__)
#define XB_ERROR(...)		::XBai::Log::GetClientLogger()->error(__VA_ARGS__)
#define XB_CRITICAL(...)	::XBai::Log::GetClientLogger()->critical(__VA_ARGS__)

#endif

#ifndef XBAI_CORE_H
#define XBAI_CORE_H

#include <memory>

	#ifdef _WIN32
		#ifdef _WIN64
			#define XB_PLATFORM_WINDOWS
		#else 
			#error "x86 Builds are Not Supported!"
		#endif
	#elif defined (__APPLE__) || (__MACH__)
		#include <TargetConditionals.h>
		#if TARGET_IPHONE_SIMULATOR == 1
			#error "IOS simulator is not supported!"
		#elif TARGET_OS_IPHONE == 1
			#define XB_PLATFORM_IOS
			#error "IOS is not supported!"
		#elif TARGET_OS_MAC == 1
			#define XB_PLATFORM_MACOS
			#error "MacOS is not supported!"
		#else 
			#error "Unknow Apple platform!"
		#endif
	#elif defined(__ANDROID__)
		#define XB_PLATFORM_ANDROID
		#error "Android is not supported!"
	#elif defined(__linux__)
		#define XB_PLATFORM_LINUX
		#error "Linux is not supported!"
	#else
		#error "Unknow platform!"
	#endif

	#ifdef XB_PLATFORM_WINDOWS
		#ifdef XB_ENABLE_DYNAMIC_DLL
			#ifdef XB_BUILD_DLL
				#define XB_API __declspec(dllexport)
			#else
				#define XB_API __declspec(dllimport)
			#endif
		#else 
			#define XB_API
		#endif
	#else 
		#error XBai only support Windows! 
	#endif

	#ifdef XB_DEBUG
		#if defined(XB_PLATFORM_WINDOWS)
			#define XB_DEBUGBREAK() __debugbreak()
		#elif defined(XB_PLATFORM_LINUX)
			#include <signal.h>
			#define XB_DEBUGBREAK() raise(SIGTRAP)
		#else
			#error "Platform dosen't support debugbreak yet!"
		#endif
		#define XB_ENABLE_ASSERT
	#else
		#define XB_DEBUGBREAK()
	#endif

	#ifdef XB_ENABLE_ASSERT
		#define XB_ASSERT(x, ...) { if (!(x)) { XB_ERROR("Assert Failed: {0}", __VA_ARGS__);__debugbreak();}}
		#define XB_CORE_ASSERT(x, ...) { if (!(x)) { XB_CORE_ERROR("Assert Failed: {0}", __VA_ARGS__);__debugbreak();}}
	#else
		#define XB_ASSERT(x, ...)
		#define XB_CORE_ASSERT(x, ...)
	#endif 

	#define BIT(x) (1 << x)
	#define XB_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }
	//#define XB_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

namespace XBai
{
	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&&  ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&&  ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}

#endif


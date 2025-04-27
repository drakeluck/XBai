#ifndef XBAI_CORE_H
#define XBAI_CORE_H
	
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
		#define XB_ENABLE_ASSERT
	#endif

	#ifdef XB_ENABLE_ASSERT
		#define XB_ASSERT(x, ...) { if (!(x)) { XB_ERROR("Assert Failed: {0}", __VA_ARGS__);__debugbreak();}}
		#define XB_CORE_ASSERT(x, ...) { if (!(x)) { XB_CORE_ERROR("Assert Failed: {0}", __VA_ARGS__);__debugbreak();}}
	#else
		#define XB_ASSERT(x, ...)
		#define XB_CORE_ASSERT(x, ...)
	#endif 

	#define BIT(x) (1 << x)
	#define XB_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

#endif


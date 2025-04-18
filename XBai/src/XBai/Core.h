#ifndef XBAI_CORE
#define XBAI_CORE

	#ifdef XB_PLATFORM_WINDOWS
		#ifdef XB_BUILD_DLL
			#define XB_API __declspec(dllexport)
		#else
			#define XB_API __declspec(dllimport)
		#endif
	#else 
		#error XBai only support Windows! 
	#endif
#endif



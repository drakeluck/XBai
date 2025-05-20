#ifndef XB_ENTRY_POINT
#define XB_ENTRY_POINT
	#ifdef XB_PLATFORM_WINDOWS

	#include "Application.h"

	extern XBai::Application* XBai::CreateApplication(ApplicationCommandLineArgs args);

	int main(int arg, char** argStr)
	{
		SetConsoleOutputCP(CP_UTF8);
		XBai::Log::Init();

		XB_PROFILE_BEGIN_SESSION("Startup", "XBaiProfile-Startup.json");
		auto app = XBai::CreateApplication({ arg, argStr });
		XB_PROFILE_END_SESSION();

		XB_PROFILE_BEGIN_SESSION("Runtime", "XBaiProfile-Runtime.json");
		app->run();
		XB_PROFILE_END_SESSION();

		XB_PROFILE_BEGIN_SESSION("Shutdown", "XBaiProfile-Shutdown.json");
		delete app;
		XB_PROFILE_END_SESSION();
	}
	#endif
#endif

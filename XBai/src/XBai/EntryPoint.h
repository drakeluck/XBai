#ifndef XB_ENTRY_POINT
#define XB_ENTRY_POINT
	#ifdef XB_PLATFORM_WINDOWS

	#include "Application.h"

	extern XBai::Application* XBai::CreateApplication();

	int main(int arg, char** argStr)
	{
		SetConsoleOutputCP(CP_UTF8);
		XBai::Log::Init();
		XB_CORE_WARN("Init!");

		auto app = XBai::CreateApplication();
		app->run();
		delete app;
	}
	#endif
#endif

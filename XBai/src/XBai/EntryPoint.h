#ifndef XB_ENTRY_POINT
#define XB_ENTRY_POINT
	#ifdef XB_PLATFORM_WINDOWS


#include "Application.h"

extern XBai::Application* XBai::CreateApplication();

int main(int arg, char** argStr)
{
	printf("first blood! FUCK");
	auto app = XBai::CreateApplication();
	app->run();
	delete app;
}

	#endif
#endif

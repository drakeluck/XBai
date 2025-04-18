#ifndef XBAI_APPLICATION
#define XBAI_APPLICATION

#include "Core.h"

namespace XBai
{
	class XB_API Application
	{
	public :
		Application();
		//析构函数被声明为虚函数，可以帮助子类正确释放资源
		virtual ~Application();

		void run();
	};

	Application* CreateApplication();

}

#endif


#ifndef XBAI_APPLICATION
#define XBAI_APPLICATION

#include "Core.h"

namespace XBai
{
	class XB_API Application
	{
	public :
		Application();
		//��������������Ϊ�麯�������԰���������ȷ�ͷ���Դ
		virtual ~Application();

		void run();
	};

	Application* CreateApplication();

}

#endif


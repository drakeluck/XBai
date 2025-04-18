#include <XBai.h>

class SandBox :public XBai::Application
{
public:
	SandBox()
	{
		
	}

	~SandBox() override
	{
		
	}
};

XBai::Application* XBai::CreateApplication()
{
	return new SandBox();
}
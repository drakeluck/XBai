#include <XBai.h>

class ExampleLayer : public XBai::Layer
{
public:
	ExampleLayer() : Layer("Example")
	{
		
	}

	void OnUpdate() override
	{
		//XB_INFO("ExampleLayer::Update");
	}

	void OnEvent(XBai::Event& e) override
	{
		//XB_INFO("{0}", e.ToString());
	}

};


class SandBox :public XBai::Application
{
public:
	SandBox()
	{
		PushLayer(new ExampleLayer());
		
	}

	~SandBox() override
	{
		
	}
};

XBai::Application* XBai::CreateApplication()
{
	return new SandBox();
}
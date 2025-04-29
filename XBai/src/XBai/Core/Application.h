#ifndef XBAI_APPLICATION_H
#define XBAI_APPLICATION_H

#include "Core.h"

#include "Window.h"
#include "LayerStack.h"
#include "XBai/Events/Event.h"
#include "XBai/Events/ApplicationEvent.h"
#include "XBai/Core/TimeStep.h"

#include "XBai/ImGui/ImGuiLayer.h"

namespace XBai
{
	class XB_API Application
	{
	public :
		Application();
		//析构函数被声明为虚函数，可以帮助子类正确释放资源
		virtual ~Application();

		void run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);

		void PushOverlay(Layer* layer);

		inline static Application& Get() { return *s_Instance; }

		inline Window& GetWindow() { return *m_Window; }

	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
	private:
		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		bool m_Minimized = false;
		TimeStep m_Timestep;
		float m_LastFrameTime = 0.0f;
		LayerStack m_LayerStack;

	private:
		static Application* s_Instance;
	};

	Application* CreateApplication();

}

#endif


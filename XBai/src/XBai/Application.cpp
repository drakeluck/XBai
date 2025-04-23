#include "xbpch.h"
#include "Application.h"
#include "XBai/Events/ApplicationEvent.h"
#include <GLFW/glfw3.h>

namespace XBai
{
	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		XB_CORE_ASSERT(!s_Instance, "Application is already exist!");
		s_Instance = this;
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(XB_BIND_EVENT_FN(Application::OnEvent));
	}

	Application::~Application()
	{
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer)
	{
		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(XB_BIND_EVENT_FN(Application::OnWindowClose));

		//XB_CORE_TRACE("{0}", e.ToString());

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
		{
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	void Application::run()
	{
		WindowResizeEvent e(1200, 900);
		XB_TRACE(e.ToString());
		while (m_Running)
		{
			glClearColor(0.5f, 0.4f, 0.1f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
			for (Layer* layer : m_LayerStack)
			{
				layer->OnUpdate();
			}
			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

}

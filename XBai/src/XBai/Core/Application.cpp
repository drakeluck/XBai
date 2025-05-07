#include "xbpch.h"
#include "Application.h"
#include "XBai/Events/ApplicationEvent.h"
#include "XBai/Render/Renderer.h"

#include <GLFW/glfw3.h>

namespace XBai
{
	Application* Application::s_Instance = nullptr;

	Application::Application(const std::string& name)
	{
		XB_PROFILE_FUNCTION()
		XB_CORE_ASSERT(!s_Instance, "Application is already exist!")
		s_Instance = this;
		m_Window = Window::Create(WindowProps(name));
		m_Window->SetEventCallback(XB_BIND_EVENT_FN(Application::OnEvent));

		Renderer::Init();
		m_Window->setVSync(false);

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	}

	Application::~Application()
	{
	}

	void Application::PushLayer(Layer* layer)
	{
		XB_PROFILE_FUNCTION()

		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer)
	{
		XB_PROFILE_FUNCTION()

		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	void Application::Close()
	{
		m_Running = false;
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(XB_BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(XB_BIND_EVENT_FN(Application::OnWindowResize));

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
		{
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	void Application::run()
	{
		WindowResizeEvent e(1280, 720);
		XB_TRACE(e.ToString());
		while (m_Running)
		{
			XB_PROFILE_SCOPE("Run Loop")

			float time = (float)glfwGetTime();
			TimeStep timestep(time - m_LastFrameTime);
			m_LastFrameTime = time;

			if (!m_Minimized)
			{
				XB_PROFILE_SCOPE("LayerStack OnUpdate")
				for (Layer* layer : m_LayerStack)
				{
					layer->OnUpdate(timestep);
				}
			}

			m_ImGuiLayer->Begin();
			{
				XB_PROFILE_SCOPE("LayerStack OnImGuiRender")
				for (Layer* layer : m_LayerStack)
				{
					layer->OnImGuiRender();
				}
			}
			m_ImGuiLayer->End();

			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		XB_PROFILE_FUNCTION()

		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			m_Minimized = true;
			return false;
		}
		m_Minimized = false;
		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

		return false;
	}

}

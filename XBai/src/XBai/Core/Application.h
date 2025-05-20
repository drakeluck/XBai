#ifndef XBAI_APPLICATION_H
#define XBAI_APPLICATION_H

#include "Core.h"

#include "Window.h"
#include "LayerStack.h"
#include "XBai/Events/Event.h"
#include "XBai/Events/ApplicationEvent.h"
#include "XBai/ImGui/ImGuiLayer.h"

int main(int argc, char** argv);

namespace XBai
{
	struct ApplicationCommandLineArgs
	{
		int Count = 0;
		char** Args = nullptr;

		const char* operator[](int index) const
		{
			XB_CORE_ASSERT(index < Count, "Application 23Hang");
			return Args[index];
		}
	};

	struct ApplicationSpecification
	{
		std::string Name = "Hazel Application";
		std::string WorkingDirectory;
		ApplicationCommandLineArgs CommandLineArgs;
	};

	class XB_API Application
	{
	public :
		Application(const ApplicationSpecification& specification);
		//析构函数被声明为虚函数，可以帮助子类正确释放资源
		virtual ~Application();

		void run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);

		void PushOverlay(Layer* layer);

		void Close();

		ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }

		inline static Application& Get() { return *s_Instance; }

		const ApplicationSpecification& GetSpecification() const { return m_Specification; }

		inline Window& GetWindow() { return *m_Window; }

	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
	private:
		ApplicationSpecification m_Specification;
		Scope<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		bool m_Minimized = false;
		float m_LastFrameTime = 0.0f;
		LayerStack m_LayerStack;

	private:
		static Application* s_Instance;
		friend int ::main(int argc, char** argv);
	};

	Application* CreateApplication(ApplicationCommandLineArgs args);
}

#endif


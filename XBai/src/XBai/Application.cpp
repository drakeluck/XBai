#include "xbpch.h"
#include "Application.h"
#include "XBai/Events/ApplicationEvent.h"
//#include <glad/glad.h>
#include "XBai/Render/Renderer.h"
//#include "Input.h"

namespace XBai
{
	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		XB_CORE_ASSERT(!s_Instance, "Application is already exist!")
		s_Instance = this;
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(XB_BIND_EVENT_FN(Application::OnEvent));

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

		//temp fuck
		m_VertexArray.reset(VertexArray::Create());
		float vertice[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0, 1.0f,
			0.5f,  -0.5f, 0.0f,	1.0f, 0.0f, 1.0, 1.0f,
			0.0f,   0.5f, 0.0f,	1.0f, 0.0f, 1.0, 1.0f
		};
		m_VertexBuffer.reset(VertexBuffer::Create(vertice, sizeof(vertice)));
		BufferLayout layout = {
			{ShaderDataType::Float3, "a_Position", false},
			{ShaderDataType::Float4, "a_Color", false},
		};
		m_VertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);

		uint32_t indices[3] = { 0, 1, 2 };
		m_IndexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(m_IndexBuffer);



		m_SquareVA.reset(VertexArray::Create());
		float vertice2[3 * 4] = {
			-0.75f, -0.75f, 0.0f,
			0.75f,  -0.75f, 0.0f,
			0.75f,   0.75f, 0.0f,
			-0.75f,  0.75f, 0.0f
		};
		std::shared_ptr<VertexBuffer> squareVB;
		squareVB.reset(VertexBuffer::Create(vertice2, sizeof(vertice2)));
		BufferLayout layout2 = {
			{ShaderDataType::Float3, "a_Position", false}
		};
		squareVB->SetLayout(layout2);
		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t indices2[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<IndexBuffer> squareIB;
		squareIB.reset(IndexBuffer::Create(indices2, sizeof(indices2) / sizeof(uint32_t)));
		m_SquareVA->SetIndexBuffer(squareIB);

		const std::string vertexSrc = R"(
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
out vec4 v_Color;
void main()
{
	v_Color = a_Color;
	gl_Position = vec4(a_Position, 1.0);
}
		)";

		const std::string fragmentSrc = R"(
#version 330 core

in vec4 v_Color;
layout(location = 0) out vec4 outColor;
void main()
{
	outColor = v_Color;
}
		)";

		const std::string vertexSrc2 = R"(
#version 330 core

layout(location = 0) in vec3 a_Position;

void main()
{
	gl_Position = vec4(a_Position, 1.0);
}
		)";

		const std::string fragmentSrc2 = R"(
#version 330 core

out vec4 outColor;
void main()
{
	outColor = vec4(0.2, 0.3, 0.8, 1.0);
}
		)";
		
		m_Shader.reset(new Shader(vertexSrc, fragmentSrc));
		m_BlueShader.reset(new Shader(vertexSrc2, fragmentSrc2));
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
			RenderCommend::SetClearColor(glm::vec4(0.2f, 0.2f, 0.2f, 1.0f));
			RenderCommend::Clear();

			Renderer::BeginScene();

			m_BlueShader->Bind();
			Renderer::Submit(m_SquareVA);

			m_Shader->Bind();
			Renderer::Submit(m_VertexArray);

			Renderer::EndScene();


			for (Layer* layer : m_LayerStack)
			{
				layer->OnUpdate();
			}

			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
			{
				layer->OnImGuiRender();
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

}

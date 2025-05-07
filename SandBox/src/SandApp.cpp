#include <XBai.h>
//入口点
#include "XBai/Core/EntryPoint.h"
#include "glm/gtc/matrix_transform.hpp"

#include "Platform/OpenGL/OpenGLShader.h"
#include "XBai/Render/Shader.h"
#include <imgui/imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Sandbox2D.h"

class ExampleLayer : public XBai::Layer
{
public:
	ExampleLayer() :
		Layer("Example"),
		m_CameraController(1280.0f / 720.0f, true)
	{
		m_VertexArray = XBai::VertexArray::Create();
		float vertice[3 * 7] = {
			-1.5f, -1.5f, 0.0f, 1.0f, 0.0f, 1.0, 1.0f,
			 1.5f, -1.5f, 0.0f,	1.0f, 0.0f, 1.0, 1.0f,
			 0.0f,  1.5f, 0.0f,	1.0f, 0.0f, 1.0, 1.0f
		};
		m_VertexBuffer = XBai::VertexBuffer::Create(vertice, sizeof(vertice));
		XBai::BufferLayout layout = {
			{XBai::ShaderDataType::Float3, "a_Position", false},
			{XBai::ShaderDataType::Float4, "a_Color", false},
		};
		m_VertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);

		uint32_t indices[3] = { 0, 1, 2 };
		m_IndexBuffer = XBai::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
		m_VertexArray->SetIndexBuffer(m_IndexBuffer);

		m_SquareVA = XBai::VertexArray::Create();
		float vertice2[5 * 4] = {
			-0.5f,  -0.5f, 0.0f,  0.0f, 0.0f,
			 0.5f,  -0.5f, 0.0f,  1.0f, 0.0f,
			 0.5f,   0.5f, 0.0f,  1.0f, 1.0f,
			-0.5f,   0.5f, 0.0f,  0.0f, 1.0f
		};
		XBai::Ref<XBai::VertexBuffer> squareVB;
		squareVB = XBai::VertexBuffer::Create(vertice2, sizeof(vertice2));
		XBai::BufferLayout layout2 = {
			{XBai::ShaderDataType::Float3, "a_Position", false},
			{XBai::ShaderDataType::Float2, "a_Texcoord", false}
		};
		squareVB->SetLayout(layout2);
		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t indices2[6] = { 0, 1, 2, 2, 3, 0 };
		XBai::Ref<XBai::IndexBuffer> squareIB;
		squareIB = XBai::IndexBuffer::Create(indices2, sizeof(indices2) / sizeof(uint32_t));
		m_SquareVA->SetIndexBuffer(squareIB);

		auto backgroundShader = m_ShaderLibrary.Load("assets/shaders/Background.glsl");
		auto squareShader = m_ShaderLibrary.Load("assets/shaders/Texture.glsl");

		m_Texture = XBai::Texture2D::Create("assets/textures/Checkerboard.png");
		std::dynamic_pointer_cast<XBai::OpenGLShader>(squareShader)->Bind();
		m_Texture->Bind(0);
		std::dynamic_pointer_cast<XBai::OpenGLShader>(squareShader)->UploadUniformInt("u_Texture", 0);
	}

	void OnUpdate(XBai::TimeStep ts) override
	{
		//XB_TRACE("Delta time : {0} ms", ts.GetMillseconds());

		m_CameraController.OnUpdate(ts);

		XBai::RenderCommand::SetClearColor(glm::vec4(0.2f, 0.2f, 0.2f, 1.0f));
		XBai::RenderCommand::Clear();

		XBai::Renderer::BeginScene(m_CameraController.GetCamera());

		auto backgroundShader = m_ShaderLibrary.Get("Background");
		XBai::Renderer::Submit(backgroundShader, m_VertexArray, glm::mat4(1.0f));

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
		auto squareShader = m_ShaderLibrary.Get("Texture");
		for (int y = 0; y < 20; y++)
		{
			for (int x = 0; x < 20; x++)
			{
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;

				XBai::Renderer::Submit(squareShader, m_SquareVA, transform);
			}
		}

		XBai::Renderer::Submit(squareShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		XBai::Renderer::EndScene();
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("square color", glm::value_ptr(m_SquareColor));

		ImGui::End();
	}

	void OnEvent(XBai::Event& e) override
	{
		m_CameraController.OnEvent(e);
	}

private:
	XBai::ShaderLibrary m_ShaderLibrary;
	//XBai::Ref<XBai::Shader> m_Shader;
	XBai::Ref<XBai::VertexBuffer> m_VertexBuffer;
	XBai::Ref<XBai::IndexBuffer> m_IndexBuffer;
	XBai::Ref<XBai::VertexArray> m_VertexArray;

	//XBai::Ref<XBai::Shader> m_SquareShader;
	XBai::Ref<XBai::VertexArray> m_SquareVA;

	XBai::Ref<XBai::Texture2D> m_Texture;
	XBai::OrthographicCameraController m_CameraController;

	glm::vec3 m_SquareColor = {0.2f, 0.3f, 0.8f};

};


class SandBox :public XBai::Application
{
public:
	SandBox()
	{
		//PushLayer(new ExampleLayer());
		PushLayer(new Sandbox2D());
	}

	~SandBox() override
	{
		
	}
};

XBai::Application* XBai::CreateApplication()
{
	return new SandBox();
}
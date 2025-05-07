#include "Sandbox2D.h"

#include "imgui/imgui.h"
#include "XBai/Render/Texture.h"
#include <Platform/OpenGL/OpenGLShader.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

static const uint32_t s_MapWidth = 24;
static const uint32_t s_MapHeight = 24;

static const char* s_MapTiles =
"WWWWWWWWWWWWWWWWWWWWWWWW"
"WDDDDDDDDDDDDDDDDDDDDDDW"
"WDDDDDDDDDDDDDDDDDDDDDDW"
"WDDDDDDDDDDDDDDDDDDDDDDW"
"WDDDDDDDDDDDDDDDDDDDDDDW"
"WDDDDDDDDDDDDDDDDDDDDDDW"
"WDDDDDDDDDDDDDDDDDDDDDDW"
"WWWWWWWWWWWWWWWWWWWWWWWW"
"WWWWWWWWWWWWWWWWWWWWWWWW"
"WWWWWWWWWWWDDWWWWWWWWWWW"
"WWWWWWWWWWWDDWWWWWWWWWWW"
"WWWWWWWWWWWDDWWWWWWWWWWW"
"WWWWWWWWWWWDDWWWWWWWWWWW"
"WWWWWWWWWWWDDWWWWWWWWWWW"
"WWWWWWWWWWWDDWWWWWWWWWWW"
"WWWWWWWWWDDDDDDWWWWWWWWW"
"WWWWWWWWWWDDDDWWWWWWWWWW"
"WWWWWWWWWWWDDWWWWWWWWWWW"
"WWWWWWWWWWWWWWWWWWWWWWWW"
"WWWWWWWWWWWWWWWWWWWWWWWW"
"WWWWWWWWWWWWWWWWWWWWWWWW"
"WWWWWWWWWWWWWWWWWWWWWWWW"
"WWWWWWWWWWWWWWWWWWWWWWWW"
"WWWWWWWWWWWWWWWWWWWWWWWW"
;

Sandbox2D::Sandbox2D() : Layer("Sandbox2D")
	, m_CameraController(1280.0f / 720.0f, true)
	, m_ParticleSystem(10000)
{
	
}

void Sandbox2D::OnAttach()
{
	XB_PROFILE_FUNCTION()

	XBai::Renderer2D::Init();
	m_CheckerboardTexture = XBai::Texture2D::Create("assets/textures/Checkerboard.png");
	m_SpriteSheet = XBai::Texture2D::Create("assets/textures/tinytown/Tilemap/tilemap_packed.png");
	s_TextureMap['W'] = XBai::SubTexture2D::CreateFromCoords(m_SpriteSheet, {1, 10}, {16, 16});
	s_TextureMap['D'] =  XBai::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 7, 7 }, { 16, 16 });

	XBai::FrameBufferSpecification spec;
	spec.Width = 1280;
	spec.Height = 720;
	//m_FrameBuffer = XBai::FrameBuffer::Create(spec);


	m_Particle.ColorBegin = {254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f};
	m_Particle.ColorEnd = { 254 / 255.0f, 209 / 255.0f, 41 / 255.0f, 1.0f };
	m_Particle.SizeBegin = 0.2f;
	m_Particle.SizeVariation = 0.05f;
	m_Particle.SizeEnd = 0.0f;
	m_Particle.LifeTime = 1.0f;
	m_Particle.Velocity = {0.0f, 0.0f};
	m_Particle.VelocityVariation = {2.0f, 1.0f};
	m_Particle.Position = {0.0f, 0.0f};
}

void Sandbox2D::OnDetach()
{
	XB_PROFILE_FUNCTION()
}

void Sandbox2D::OnUpdate(XBai::TimeStep ts)
{
	XB_PROFILE_FUNCTION()

	m_CameraController.OnUpdate(ts);

	XBai::Renderer2D::ResetStats();
	
	{
		//XB_PROFILE_SCOPE("Clear Color") --0.001ms
		XBai::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		XBai::RenderCommand::Clear();
	}

	{
		XB_PROFILE_SCOPE("SandBox2D::Every Scene")
		XBai::Renderer2D::BeginScene(m_CameraController.GetCamera());

		//棋盘背景
		//XBai::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.3f }, { 20.0f, 20.0f }, m_CheckerboardTexture, 10.0f);

		for (uint32_t y = 0; y < s_MapHeight; y++)
		{
			for (uint32_t x = 0; x < s_MapWidth; x++)
			{
				char tiletype = s_MapTiles[y * s_MapWidth + x];
				if (s_TextureMap.find(tiletype) != s_TextureMap.end())
				{
					XBai::Renderer2D::DrawQuad({ x - s_MapWidth / 2.0f, s_MapHeight - y - s_MapHeight / 2.0f, 0.1f }, { 1.0f, 1.0f }, s_TextureMap[tiletype]);
				}
			}
		}
		// //不旋转 不带纹理
		// XBai::Renderer2D::DrawQuad({0.26f, 0.41f, 0.97f}, {0.3f, 0.6f}, { 0.8f, 0.6f, 0.1f, 1.0f });
		// //不旋转 带纹理
		//XBai::Renderer2D::DrawQuad({ -1.0f, 0.0f,  0.0f }, { 1.8f, 1.8f }, m_SpriteSheet);
		// //旋转 带纹理
		// XBai::Renderer2D::DrawRotatedQuad({ -0.3f, 0.0f, - 0.2f }, { 0.4f, 0.4f }, 64.0f, m_CheckerboardTexture, 5.0f);
		// //旋转 不带纹理(这里的旋转是弧度)
		// XBai::Renderer2D::DrawRotatedQuad({ -0.4f, 0.0f, - 0.2f }, { 0.4f, 0.4f }, glm::radians(16.0f), { 0.2f, 0.8f, 0.4f, 1.0f });
		XBai::Renderer2D::EndScene();
	}
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Settings");

	auto stats = XBai::Renderer2D::GetStats();
	ImGui::Text("Renderer2D Stats:");
	ImGui::Text("Draw Calls: %d", stats.DrawCalls);
	ImGui::Text("Quad Count: %d", stats.QuadCount);
	ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
	ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

	ImGui::End();

}

void Sandbox2D::OnEvent(XBai::Event& e)
{
	// 事件
	m_CameraController.OnEvent(e);
}

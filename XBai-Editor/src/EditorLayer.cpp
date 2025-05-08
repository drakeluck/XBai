#include "xbpch.h"
#include "EditorLayer.h"
#include "imgui/imgui.h"
#include "XBai/Scene/SceneSerializer.h"
#include "XBai/Utils/PlatformUtils.h"
#include <Platform/OpenGL/OpenGLShader.h>
#include <glm/gtc/type_ptr.hpp>

namespace XBai
{
	EditorLayer::EditorLayer() : Layer("EditorLayer")
		, m_CameraController(1280.0f / 720.0f, true)
		
	{

	}

	void EditorLayer::OnAttach()
	{
		XB_PROFILE_FUNCTION()

		Renderer2D::Init();
		FrameBufferSpecification spec;
		spec.Width = 1280;
		spec.Height = 720;
		m_FrameBuffer = XBai::FrameBuffer::Create(spec);
		m_ActiveScene = CreateRef<Scene>();
		m_SceneHierarchyPanel.SetContext(m_ActiveScene);
	}

	void EditorLayer::OnDetach()
	{
		XB_PROFILE_FUNCTION()
	}

	void EditorLayer::OnUpdate(TimeStep ts)
	{
		XB_PROFILE_FUNCTION()

		//Resize
		if (FrameBufferSpecification spec = m_FrameBuffer->GetSpecification();
			m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f 
			&& (spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
		{
			m_FrameBuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_CameraController.OnResize(m_ViewportSize.x, m_ViewportSize.y);

			m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		}

		if (m_ViewportFocused)
			m_CameraController.OnUpdate(ts);

		Renderer2D::ResetStats();
		
		//XB_PROFILE_SCOPE("Clear Color") --0.001ms
		m_FrameBuffer->Bind();
		RenderCommand::SetClearColor({ 0.15f, 0.15f, 0.15f, 1 });
		RenderCommand::Clear();
		
		m_ActiveScene->OnUpdate(ts);

		m_FrameBuffer->Unbind();
	}

	void EditorLayer::OnImGuiRender()
	{
		static bool opt_fullscreen = true;
		static bool opt_padding = false;

		static bool dockspaceOpen = true;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;


		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			const ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->WorkPos);
			ImGui::SetNextWindowSize(viewport->WorkSize);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}
		else
		{
			dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
		}
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		if (!opt_padding)
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
		if (!opt_padding)
			ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// Submit the DockSpace
		ImGuiIO& io = ImGui::GetIO();
		ImGuiStyle& style = ImGui::GetStyle();
		float minWinSizeX = style.WindowMinSize.x;
		style.WindowMinSize.x = 350.0f;
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}
		style.WindowMinSize.x = minWinSizeX;

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New", "Ctrl+N"))
				{
					NewScene();
				}
				if (ImGui::MenuItem("Open...", "Ctrl+O"))
				{
					OpenScene();
				}
				if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
				{
					SaveSceneAs();
				}
				if (ImGui::MenuItem("Close")) Application::Get().Close();
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}
		m_SceneHierarchyPanel.OnImGuiRender();

		ImGui::Begin("Status");

		auto stats = XBai::Renderer2D::GetStats();
		ImGui::Text("Renderer2D Stats:");
		ImGui::Text("Draw Calls: %d", stats.DrawCalls);
		ImGui::Text("Quad Count: %d", stats.QuadCount);
		ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
		ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});
		ImGui::Begin("ViewPort");

		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();
		Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused || !m_ViewportHovered);

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		if (m_ViewportSize.x != viewportPanelSize.x || m_ViewportSize.y != viewportPanelSize.y)
		{
			m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
			m_CameraController.OnResize(viewportPanelSize.x, viewportPanelSize.y);
		}

		ImGui::Image(m_FrameBuffer->GetColorAttachmentRendererID(), ImVec2{m_ViewportSize.x, m_ViewportSize.y}, { 0.0f, 1.0f }, { 1.0f, 0.0f });
		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::End();
	}

	void EditorLayer::OnEvent(Event& e)
	{
		// 事件
		m_CameraController.OnEvent(e);

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressedEvent>(XB_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
	}

	bool EditorLayer::OnKeyPressed(KeyPressedEvent& e)
	{
		if (e.GetRepeatCount() > 0)
		{
			return false;
		}
		bool control = Input::IsKeyPressed(XB_KEY_LEFT_CONTROL) || Input::IsKeyPressed(XB_KEY_RIGHT_CONTROL);
		bool shift = Input::IsKeyPressed(XB_KEY_LEFT_SHIFT) || Input::IsKeyPressed(XB_KEY_RIGHT_SHIFT);
		switch (e.GetKeyCode())
		{
		case XB_KEY_N:
			if (control)
			{
				NewScene();
			}
			break;
		case XB_KEY_O:
			if (control)
			{
				OpenScene();
			}
			break;

		case XB_KEY_S:
			if (control && shift)
			{
				SaveSceneAs();
			}
			break;
		}
	}

	void EditorLayer::NewScene()
	{
		m_ActiveScene = CreateRef<Scene>();
		m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		m_SceneHierarchyPanel.SetContext(m_ActiveScene);
	}

	void EditorLayer::OpenScene()
	{
		std::string filepath = FileDialogs::OpenFile("XBai Scene (*.xbai)\0*.xbai\0");
		if (!filepath.empty())
		{
			m_ActiveScene = CreateRef<Scene>();
			m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_SceneHierarchyPanel.SetContext(m_ActiveScene);
			SceneSerializer serializer(m_ActiveScene);
			serializer.Deserialize(filepath);
		}
	}

	void EditorLayer::SaveSceneAs()
	{
		std::string filepath = FileDialogs::SaveFile("XBai Scene (*.xbai)\0*.xbai\0");
		if (!filepath.empty())
		{
			SceneSerializer serializer(m_ActiveScene);
			serializer.Serialize(filepath);
		}
	}
}



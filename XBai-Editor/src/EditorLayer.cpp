#include "xbpch.h"
#include "EditorLayer.h"
#include "imgui/imgui.h"
#include "XBai/Scene/SceneSerializer.h"
#include "XBai/Utils/PlatformUtils.h"
#include "ImGuizmo/ImGuizmo.h"
#include "XBai/Math/Math.h"
#include <glm/gtc/type_ptr.hpp>

namespace XBai
{
	extern const std::filesystem::path g_Assetpath;

	EditorLayer::EditorLayer() : Layer("EditorLayer")
		//, m_CameraController(1280.0f / 720.0f, true)
	{

	}

	EditorLayer::~EditorLayer()
	{

	}

	void EditorLayer::OnAttach()
	{
		XB_PROFILE_FUNCTION()

		Renderer2D::Init();

		m_IconPlay = Texture2D::Create("Resources/Icons/PlayButton.png");
		m_IconSimulate = Texture2D::Create("Resources/Icons/SimulateButton.png");
		m_IconStop = Texture2D::Create("Resources/Icons/StopButton.png");

		FrameBufferSpecification fbSpec;
		fbSpec.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth };
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		m_FrameBuffer = XBai::FrameBuffer::Create(fbSpec);
		m_EditorScene = CreateRef<Scene>();
		m_ActiveScene = m_EditorScene;

		auto commandLineArgs = Application::Get().GetSpecification().CommandLineArgs;
		if (commandLineArgs.Count > 1)
		{
			auto sceneFilePath = commandLineArgs[1];
			SceneSerializer serializer(m_ActiveScene);
			serializer.Deserialize(sceneFilePath);
		}

		m_EditorCamera = EditorCamera(30.0f, 1.778f, 0.1f, 1000.0f);
		
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

			m_EditorCamera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);

			m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		}

		m_EditorCamera.OnUpdate(ts);

		Renderer2D::ResetStats();
		
		//XB_PROFILE_SCOPE("Clear Color") --0.001ms
		m_FrameBuffer->Bind();
		RenderCommand::SetClearColor({ 0.15f, 0.15f, 0.15f, 1 });
		RenderCommand::Clear();

		m_FrameBuffer->ClearAttachment(1, -1);

		switch(m_SceneState)
		{
		case SceneState::Edit:
			m_EditorCamera.OnUpdate(ts);
			m_ActiveScene->OnUpdateEditor(ts, m_EditorCamera);
			break;
		case SceneState::Play:
			m_ActiveScene->OnUpdateRuntime(ts);
			break;
		case SceneState::Simulate:
			m_EditorCamera.OnUpdate(ts);
			m_ActiveScene->OnUpdateSimulation(ts, m_EditorCamera);
			break;
		}
		
		auto [mx, my] = ImGui::GetMousePos();
		mx -= m_ViewportBounds[0].x;
		my -= m_ViewportBounds[0].y;
		glm::vec2 viewportSize = m_ViewportBounds[1] - m_ViewportBounds[0];
		my = viewportSize.y - my;
		int mouseX = (int)mx;
		int mouseY = (int)my;
		if (mouseX >= 0 && mouseY >= 0 && mouseX <= (int)viewportSize.x && mouseY <= (int)viewportSize.y)
		{
			int pixelData = m_FrameBuffer->ReadPixel(1, mouseX, mouseY);
			if (pixelData == -1 || pixelData < 0 || pixelData > 10000000)
				m_HoveredEntity = {};
			else
				m_HoveredEntity = { (entt::entity)pixelData, m_ActiveScene.get() };
		}
		OnOverlayRender();
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
		m_ContentBrowserPanel.OnImGuiRender();

		ImGui::Begin("Status");

		std::string name = "None";
		if (m_HoveredEntity)
		{
			name = m_HoveredEntity.GetComponent<TagComponent>().Tag;
		}
		ImGui::Text("Hovered Entity : %s", name.c_str());

		auto stats = XBai::Renderer2D::GetStats();
		ImGui::Text("Renderer2D Stats:");
		ImGui::Text("Draw Calls: %d", stats.DrawCalls);
		ImGui::Text("Quad Count: %d", stats.QuadCount);
		ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
		ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
		ImGui::End();

		ImGui::Begin("设置");
		ImGui::Checkbox("是否显示碰撞体框", &m_ShowPhysicsColliders);

		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});
		ImGui::Begin("ViewPort");

		auto viewportOffset = ImGui::GetCursorPos();//这里的位置包含了标签栏
		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();
		Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused && !m_ViewportHovered);

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		if (m_ViewportSize.x != viewportPanelSize.x || m_ViewportSize.y != viewportPanelSize.y)
		{
			m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
			//m_CameraController.OnResize(viewportPanelSize.x, viewportPanelSize.y);
		}

		ImGui::Image(m_FrameBuffer->GetColorAttachmentRendererID(), ImVec2{m_ViewportSize.x, m_ViewportSize.y}, { 0.0f, 1.0f }, { 1.0f, 0.0f });

		auto windowSize = ImGui::GetWindowSize();
		ImVec2 minBound = ImGui::GetWindowPos();
		minBound.x += viewportOffset.x;
		minBound.y += viewportOffset.y;

		ImVec2 maxBound = { minBound.x + windowSize.x, minBound.y + windowSize.y };
		m_ViewportBounds[0] = { minBound.x, minBound.y };
		m_ViewportBounds[1] = { maxBound.x, maxBound.y };

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
			{
				const wchar_t* path = (const wchar_t*)payload->Data;
				OpenScene(std::filesystem::path(g_Assetpath) / path);
			}
			ImGui::EndDragDropTarget();
		}

		Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
		if (selectedEntity && m_GizmoType != -1)
		{
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();
			float windowWidth = ImGui::GetWindowWidth();
			float windowHeight = ImGui::GetWindowHeight();
			ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

			//Editor相机
			const glm::mat4& cameraProjection = m_EditorCamera.GetProjection();
			glm::mat4 cameraView = m_EditorCamera.GetViewMatrix();

			auto& tc = selectedEntity.GetComponent<TransformComponent>();
			glm::mat4 transform = tc.GetTransform();

			//是否对齐的控制键
			bool snap = Input::IsKeyPressed(XB_KEY_LEFT_CONTROL);
			float snapValue = 0.5f;
			if (m_GizmoType == ImGuizmo::OPERATION::ROTATE)
			{
				snapValue = 45.0f;
			}
			float snapValues[3] = {snapValue, snapValue, snapValue};

			ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection), 
				(ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform), nullptr, snap ? snapValues : nullptr);
			if (ImGuizmo::IsUsing())
			{
				glm::vec3 translation, rotation, scale;
				Math::DecomposeTransform(transform, translation, rotation, scale);

				glm::vec3 originalRotation = tc.Rotation;
				glm::vec3 deltaRotation = rotation - tc.Rotation;

				tc.Translation = translation;
				tc.Rotation += deltaRotation;
				tc.Scale = scale;
			}
		}

		ImGui::End();
		ImGui::PopStyleVar();

		UI_Toolbar();

		ImGui::End();
	}

	void EditorLayer::UI_Toolbar()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 2));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		auto& colors = ImGui::GetStyle().Colors;
		const auto& buttonHovered = colors[ImGuiCol_ButtonHovered];
		const auto& buttonActive = colors[ImGuiCol_ButtonActive];
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(buttonHovered.x, buttonHovered.y, buttonHovered.z, 0.5f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(buttonActive.x, buttonActive.y, buttonActive.z, 0.5f));

		ImGui::Begin("##toolbar", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
		float size = ImGui::GetWindowHeight() - 4.0f;

		bool toolbarEnabled = (bool)m_ActiveScene;
		{
			Ref<Texture2D> icon = m_SceneState == SceneState::Play ?  m_IconStop : m_IconPlay;
			ImGui::SetCursorPosX(ImGui::GetWindowContentRegionMax().x * 0.3f - (size * 0.5f));
			if (ImGui::ImageButton("SceneState", icon->GetRendererID(), ImVec2(size, size), {0, 1}, {1, 0}) && toolbarEnabled)
			{
				if (m_SceneState == SceneState::Edit || m_SceneState == SceneState::Simulate)
					OnScenePlay();
				else if (m_SceneState == SceneState::Play)
					OnSceneStop();
			}
		}
		ImGui::SameLine();
		{
			Ref<Texture2D> icon = m_SceneState == SceneState::Simulate ? m_IconStop : m_IconSimulate;
			//ImGui::SetCursorPosX(ImGui::GetWindowContentRegionMax().x * 0.6f - (size * 0.5f));
			if (ImGui::ImageButton("SceneSimulationState", icon->GetRendererID(), ImVec2(size, size), { 0, 1 }, { 1, 0 }) && toolbarEnabled)
			{
				if (m_SceneState == SceneState::Edit || m_SceneState == SceneState::Play)
					OnSceneSimulate();
				else if (m_SceneState == SceneState::Simulate)
					OnSceneStop();
			}
		}

		ImGui::PopStyleVar(2);
		ImGui::PopStyleColor(3);
		ImGui::End();
	}

	void EditorLayer::OnEvent(Event& e)
	{
		// 事件
		//m_CameraController.OnEvent(e);
		m_EditorCamera.OnEvent(e);

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressedEvent>(XB_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
		dispatcher.Dispatch<MouseButtonPressedEvent>(XB_BIND_EVENT_FN(EditorLayer::OnMouseButtonPressed));
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
				NewScene();
			break;
		case XB_KEY_O:
			if (control)
				OpenScene();
			break;

		case XB_KEY_S:
			if (control)
			{
				if (shift)
					SaveSceneAs();
				else
					SaveScene();
			}
			break;

		case XB_KEY_D:
			if (control)
				OnDuplicateEntity();
			break;
		case XB_KEY_Q:
			m_GizmoType = -1;
			break;
		case XB_KEY_W:
			m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
			break;
		case XB_KEY_E:
			m_GizmoType = ImGuizmo::OPERATION::ROTATE;
			break;
		case XB_KEY_R:
			m_GizmoType = ImGuizmo::OPERATION::SCALE;
			break;
		}
		return false;
	}

	bool EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e)
	{
		if (e.GetMouseButton() == XB_MOUSE_BUTTON_LEFT)
		{
			if (m_ViewportHovered && !ImGuizmo::IsOver() && !Input::IsKeyPressed(XB_KEY_LEFT_ALT))
			{
				m_SceneHierarchyPanel.SetSelectedEntity(m_HoveredEntity);
			}
		}
		return false;
	}

	void EditorLayer::OnOverlayRender()
	{
		if (m_SceneState == SceneState::Play)
		{
			Entity camera = m_ActiveScene->GetPrimaryCameraEntity();
			if (!camera)
				return;
			Renderer2D::BeginScene(camera.GetComponent<CameraComponent>().Camera, camera.GetComponent<TransformComponent>().GetTransform());
		}
		else if (m_SceneState == SceneState::Edit)
		{
			Renderer2D::BeginScene(m_EditorCamera);
		}

		if (m_ShowPhysicsColliders)
		{
			{//矩形碰撞体框
				auto view = m_ActiveScene->GetAllEntitysWith<TransformComponent, BoxCollider2DComponent>();
				for (auto entity : view)
				{
					auto [tc, bc2d] = view.get<TransformComponent, BoxCollider2DComponent>(entity);
					glm::mat4 rotation = glm::toMat4(glm::quat(tc.Rotation));
					glm::vec3 scale = tc.Scale * glm::vec3(bc2d.Size * 2.0f, 1.0f);

					glm::mat4 transform = glm::translate(glm::mat4(1.0f), tc.Translation)
						* rotation
						* glm::translate(glm::mat4(1.0f), glm::vec3(bc2d.Offset, 0.001f))
						* glm::scale(glm::mat4(1.0f), scale);
					Renderer2D::DrawRect(transform, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
				}
			}

			{//圆形碰撞体框
				auto view = m_ActiveScene->GetAllEntitysWith<TransformComponent, CircleCollider2DComponent>();
				for (auto entity : view)
				{
					auto [tc, cc2d] = view.get<TransformComponent, CircleCollider2DComponent>(entity);
					glm::vec3 translate = tc.Translation + glm::vec3(cc2d.Offset, 0.001f);

					glm::mat4 rotation = glm::toMat4(glm::quat(tc.Rotation));

					glm::vec3 scale = tc.Scale * glm::vec3(cc2d.Radius * 2.0f);

					glm::mat4 transform = glm::translate(glm::mat4(1.0f), tc.Translation)
						* rotation
						* glm::translate(glm::mat4(1.0f), glm::vec3(cc2d.Offset, 0.001f))
						* glm::scale(glm::mat4(1.0f), scale);
					Renderer2D::DrawCircle(transform, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), 0.01f, 0.01f);
				}
			}
		}

		Renderer2D::EndScene();
	}

	void EditorLayer::NewScene()
	{
		m_EditorScene = CreateRef<Scene>();
		m_EditorScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		m_SceneHierarchyPanel.SetContext(m_EditorScene);
		// m_EditorCamera = EditorCamera(30.0f, 1.778f, 0.1f, 1000.0f);
		m_EditorCamera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
		m_ActiveScene = m_EditorScene;
		m_EditorScenePath = std::filesystem::path();
	}

	void EditorLayer::OpenScene()
	{
		std::string filepath = FileDialogs::OpenFile("XBai Scene (*.xbai)\0*.xbai\0");
		if (!filepath.empty())
		{
			OpenScene(filepath);
		}
	}

	void EditorLayer::OpenScene(const std::filesystem::path& path)
	{
		if (m_SceneState != SceneState::Edit)
			OnSceneStop();

		Ref<Scene> newScene = CreateRef<Scene>();
		SceneSerializer serializer(newScene);
		if (serializer.Deserialize(path.string()))
		{
			m_EditorScene = newScene;
			m_EditorScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_EditorCamera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
			m_SceneHierarchyPanel.SetContext(m_EditorScene);
			m_ActiveScene = m_EditorScene;
			m_EditorScenePath = path;
		}
	}

	void EditorLayer::SaveScene()
	{
		if (!m_EditorScenePath.empty())
		{
			SerializeScene(m_ActiveScene, m_EditorScenePath);
		}
		else
		{
			SaveSceneAs();
		}
	}

	void EditorLayer::SaveSceneAs()
	{
		std::string filepath = FileDialogs::SaveFile("XBai Scene (*.xbai)\0*.xbai\0");
		if (!filepath.empty())
		{
			SerializeScene(m_ActiveScene, filepath);
			m_EditorScenePath = filepath;
		}
	}

	void EditorLayer::SerializeScene(Ref<Scene> scene, const std::filesystem::path& path)
	{
		SceneSerializer serializer(scene);
		serializer.Serialize(path.string());
	}

	void EditorLayer::OnScenePlay()
	{
		if (m_SceneState == SceneState::Simulate)
		{
			OnSceneStop();
		}
		m_SceneState = SceneState::Play;

		m_ActiveScene = Scene::Copy(m_EditorScene);
		m_ActiveScene->OnRuntimeStart();

		m_SceneHierarchyPanel.SetContext(m_ActiveScene);
	}

	void EditorLayer::OnSceneStop()
	{
		if (m_SceneState == SceneState::Play)
		{
			m_ActiveScene->OnRuntimeStop();
		}
		else if (m_SceneState == SceneState::Simulate)
		{
			m_ActiveScene->OnSimulationStop();
		}
		m_SceneState = SceneState::Edit;
		m_ActiveScene = m_EditorScene;
		m_SceneHierarchyPanel.SetContext(m_ActiveScene);
	}

	void EditorLayer::OnScenePause()
	{
		m_SceneState = SceneState::Pause;
	}

	void EditorLayer::OnSceneSimulate()
	{
		if (m_SceneState == SceneState::Play)
		{
			OnSceneStop();
		}
		m_SceneState = SceneState::Simulate;

		m_ActiveScene = Scene::Copy(m_EditorScene);
		m_ActiveScene->OnSimulationStart();

		m_SceneHierarchyPanel.SetContext(m_ActiveScene);
	}

	void EditorLayer::OnDuplicateEntity()
	{
		if (m_SceneState != SceneState::Edit)
			return;
		
		if (Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity())
		{
			m_EditorScene->DuplicateEntity(selectedEntity);
		}
	}
}



﻿#ifndef XB_EDITOR_LAYER_H
#define XB_EDITOR_LAYER_H

#include "XBai.h"
#include "Panels/SceneHierarchyPanel.h"
#include "Panels/ContentBrowserPanel.h"
#include "XBai/Render/EditorCamera.h"

namespace XBai
{
	class EditorLayer : public Layer
	{

	public:
		EditorLayer();
		virtual ~EditorLayer() override;
		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual void OnUpdate(TimeStep ts) override;
		virtual void OnImGuiRender()override;
		virtual void OnEvent(Event& e)override;
	private:
		bool OnKeyPressed(KeyPressedEvent& e);
		bool OnMouseButtonPressed(MouseButtonPressedEvent& e);

		void OnOverlayRender();
		void NewScene();
		void OpenScene();
		void OpenScene(const std::filesystem::path& path);
		void SaveScene();
		void SaveSceneAs();

		void SerializeScene(Ref<Scene> scene, const std::filesystem::path& path);

		void OnScenePlay();			//播放
		void OnSceneStop();			//停止
		void OnScenePause();		//暂停
		void OnSceneSimulate();		//模拟

		void OnDuplicateEntity();

		void UI_Toolbar();
	private:
		Ref<FrameBuffer> m_FrameBuffer;

		Ref<Scene> m_ActiveScene;//当前活动场景

		Ref<Scene> m_EditorScene;//编辑时场景

		Ref<Texture2D> m_IconPlay;
		Ref<Texture2D> m_IconStop;
		Ref<Texture2D> m_IconSimulate;

		std::filesystem::path m_EditorScenePath;

		Entity m_HoveredEntity;

		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
		glm::vec2 m_ViewportBounds[2];

		EditorCamera m_EditorCamera;

		bool m_ViewportFocused = false;
		bool m_ViewportHovered = false;

		enum class SceneState
		{
			Edit = 0, Play = 1, Pause = 2, Simulate = 3
		};

		SceneState m_SceneState = SceneState::Edit;

		int m_GizmoType = -1;

		bool m_ShowPhysicsColliders = false;

		//Panels
		SceneHierarchyPanel m_SceneHierarchyPanel;
		ContentBrowserPanel m_ContentBrowserPanel;
	};
}

#endif


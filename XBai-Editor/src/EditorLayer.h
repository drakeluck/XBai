#ifndef XB_EDITOR_LAYER_H
#define XB_EDITOR_LAYER_H

#include "XBai.h"
#include "Panels/SceneHierarchyPanel.h"
#include "XBai/Render/EditorCamera.h"

namespace XBai
{
	class EditorLayer : public Layer
	{

	public:
		EditorLayer();
		virtual ~EditorLayer() override = default;
		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual void OnUpdate(TimeStep ts) override;
		virtual void OnImGuiRender()override;
		virtual void OnEvent(Event& e)override;
	private:
		bool OnKeyPressed(KeyPressedEvent& e);
		bool OnMouseButtonPressed(MouseButtonPressedEvent& e);
		void NewScene();
		void OpenScene();
		void SaveSceneAs();
	private:
		OrthographicCameraController m_CameraController;
		Ref<Shader> m_FlatShader;			// shader类 指针
		Ref<VertexArray> m_FlatVertexArray;
		Ref<Texture2D> m_CheckerboardTexture;

		Ref<Texture2D> m_SpriteSheet;
		Ref<FrameBuffer> m_FrameBuffer;

		Ref<Scene> m_ActiveScene;

		Entity m_HoveredEntity;

		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
		glm::vec2 m_ViewportBounds[2];

		EditorCamera m_EditorCamera;

		bool m_ViewportFocused = false;
		bool m_ViewportHovered = false;

		std::unordered_map<char, Ref<SubTexture2D>> s_TextureMap;

		int m_GizmoType = -1;

		//Panels
		SceneHierarchyPanel m_SceneHierarchyPanel;
	};
}

#endif


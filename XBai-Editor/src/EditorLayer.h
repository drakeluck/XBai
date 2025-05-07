#ifndef XB_EDITOR_LAYER_H
#define XB_EDITOR_LAYER_H

#include "XBai.h"
#include "Panels/SceneHierarchyPanel.h"

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
		OrthographicCameraController m_CameraController;
		Ref<Shader> m_FlatShader;			// shader类 指针
		Ref<VertexArray> m_FlatVertexArray;
		Ref<Texture2D> m_CheckerboardTexture;

		Ref<Texture2D> m_SpriteSheet;
		Ref<FrameBuffer> m_FrameBuffer;

		Ref<Scene> m_ActiveScene;
		Entity m_SquareEntity;
		Entity m_CameraEntity;
		Entity m_SecondCameraEntity;

		glm::vec2 m_ViewportSize = {1.0f, 1.0f};

		bool m_ViewportFocused = false;
		bool m_ViewportHovered = false;

		glm::vec4 m_FlatColor = { 0.2f, 0.3f, 0.8f, 1.0f };

		std::unordered_map<char, XBai::Ref<SubTexture2D>> s_TextureMap;

		//Panels
		SceneHierarchyPanel m_SceneHierarchyPanel;

	};
}

#endif


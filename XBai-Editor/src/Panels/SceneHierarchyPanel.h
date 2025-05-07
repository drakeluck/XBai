#ifndef XB_SCENE_HIERARCHY_PANEL_H
#define XB_SCENE_HIERARCHY_PANEL_H

#include "XBai/Core/Core.h"
#include "XBai/Core/Log.h"
#include "XBai/Scene/Scene.h"
#include "XBai/Scene/Entity.h"

namespace XBai
{
	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene>& scene);

		void SetContext(const Ref<Scene>& scene);

		void OnImGuiRender();

	private:
		void DrawEntityNode(Entity entity);
		void DrawComponents(Entity entity);

	private:
		Ref<Scene> m_Context;
		Entity m_SelectionContext;
	};

}

#endif

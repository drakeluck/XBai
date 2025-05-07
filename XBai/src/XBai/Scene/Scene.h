#ifndef XB_SCENE_H
#define XB_SCENE_H

#include "entt.hpp"
#include "XBai/Core/TimeStep.h"

namespace XBai
{
	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = std::string());
		void DestroyEntity(Entity entity);

		void OnUpdate(TimeStep ts);

		void OnViewportResize(uint32_t width, uint32_t height);

	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);

	private:
		uint32_t m_ViewportWidth = 0;
		uint32_t m_ViewportHeight = 0;

		entt::registry m_Registry;
		friend class Entity;
		friend class SceneHierarchyPanel;
	};
}

#endif

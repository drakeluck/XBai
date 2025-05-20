#ifndef XB_SCENE_H
#define XB_SCENE_H

#include "entt.hpp"

#include "XBai/Core/TimeStep.h"
#include "XBai/Core/UUID.h"
#include "XBai/Render/EditorCamera.h"
#include "../vendor/Box2D/include/box2d/box2d.h"

namespace XBai
{
	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();

		static Ref<Scene> Copy(Ref<Scene> other);

		Entity CreateEntity(const std::string& name = std::string());
		Entity CreateEntityWithUUID(UUID uuid, const std::string& name = std::string());
		void DestroyEntity(Entity entity);

		void OnRuntimeStart();
		void OnRuntimeStop();

		void OnSimulationStart();
		void OnSimulationStop();

		void OnUpdateEditor(TimeStep ts, EditorCamera& camera);
		void OnUpdateRuntime(TimeStep ts);
		void OnUpdateSimulation(TimeStep ts, EditorCamera& camera);

		void OnViewportResize(uint32_t width, uint32_t height);

		//重复实体
		void DuplicateEntity(Entity entity);

		Entity GetPrimaryCameraEntity();

		template<typename... Components>
		auto GetAllEntitysWith()
		{
			return m_Registry.view<Components...>();
		}

	private:
		void OnPhysics2DStart();
		void OnPhysics2DStop();

		void RenderScene(EditorCamera& camera);

	private:
		uint32_t m_ViewportWidth = 0;
		uint32_t m_ViewportHeight = 0;

		entt::registry m_Registry;

		b2WorldId m_PhysicsWorld;
		b2BodyId m_RuntimeBodyID;

		friend class Entity;
		friend class SceneSerializer;
		friend class SceneHierarchyPanel;
	};
}

#endif

#include "xbpch.h"
#include "Scene.h"

#include "Component.h"
#include "ScriptableEntity.h"
#include "Entity.h"
#include "XBai/Render/Renderer2D.h"

#include <glm/glm.hpp>

namespace XBai
{
	Scene::Scene()
	{
		
	}

	Scene::~Scene()
	{

	}

	template<typename Component>
	static void CopyComponent(entt::registry& dst, entt::registry& src, const std::unordered_map<UUID, entt::entity>& enttMap)
	{
		auto view = src.view<Component>();
		for (auto e : view)
		{
			UUID srcID = src.get<IDComponent>(e).ID;

			entt::entity dstEnttID = enttMap.at(srcID);
			dst.emplace_or_replace<Component>(dstEnttID, src.get<Component>(e));
		}
	}

	template<typename Component>
	static void CopyComponentIfExists(Entity dst, Entity src)
	{
		if (src.HasComponent<Component>())
		{
			dst.AddOrReplaceComponent<Component>(src.GetComponent<Component>());
		}
	}

	Ref<Scene> Scene::Copy(Ref<Scene> other)
	{
		Ref<Scene> newScene = CreateRef<Scene>();

		newScene->m_ViewportWidth = other->m_ViewportWidth;
		newScene->m_ViewportHeight = other->m_ViewportHeight;
		std::unordered_map<UUID, entt::entity> enttMap;

		auto& srcSceneRegistry = other->m_Registry;
		auto& dstSceneRegistry = newScene->m_Registry;
		auto idView = srcSceneRegistry.view<IDComponent>();
		for (auto e : idView)
		{
			UUID uuid = srcSceneRegistry.get<IDComponent>(e).ID;
			const auto& name = srcSceneRegistry.get<TagComponent>(e).Tag;
			Entity newEntity = newScene->CreateEntityWithUUID(uuid, name);
			enttMap[uuid] = (entt::entity)newEntity;
		}
		CopyComponent<TransformComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
		CopyComponent<SpriteRendererComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
		CopyComponent<CameraComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
		CopyComponent<NativeScriptComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
		CopyComponent<Rigibody2DComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
		CopyComponent<BoxCollider2DComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
		return newScene;
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		return CreateEntityWithUUID(UUID(), name);
	}

	Entity Scene::CreateEntityWithUUID(UUID uuid, const std::string& name)
	{
		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<IDComponent>(uuid);
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Block Entity" : name;
		return entity;
	}

	void Scene::DestroyEntity(Entity entity)
	{
		m_Registry.destroy(entity);
	}

	void Scene::OnRuntimeStart()
	{
		b2WorldDef def = b2DefaultWorldDef();
		def.gravity = { 0.0f, -9.8f };
		def.restitutionThreshold = 0.5f;
		m_PhysicsWorld = b2CreateWorld(&def);

		auto view = m_Registry.view<Rigibody2DComponent>();
		for (auto e : view)
		{
			Entity entity = { e, this };
			auto& transform = entity.GetComponent<TransformComponent>();
			auto& rb2d = entity.GetComponent<Rigibody2DComponent>();

			b2BodyDef bodydef = b2DefaultBodyDef();
			bodydef.type = (b2BodyType)rb2d.Type;
			bodydef.position = { transform.Translation.x, transform.Translation.y };
			bodydef.rotation = b2MakeRot(transform.Rotation.z);
			bodydef.fixedRotation = rb2d.FixedRotation;
			rb2d.RuntimeBody = b2CreateBody(m_PhysicsWorld, &bodydef);

			if (entity.HasComponent<BoxCollider2DComponent>())
			{
				auto& bc2d = entity.GetComponent<BoxCollider2DComponent>();
				b2Polygon dynamicBox = b2MakeBox(bc2d.Size.x * transform.Scale.x, bc2d.Size.y * transform.Scale.y);

				b2ShapeDef shapedef = b2DefaultShapeDef();
				shapedef.density = bc2d.Density;
				shapedef.material.friction = bc2d.Friction;
				shapedef.material.restitution = bc2d.Restitution;
				b2CreatePolygonShape(rb2d.RuntimeBody, &shapedef, &dynamicBox);
			}
		}
	}

	void Scene::OnRuntimeStop()
	{
		b2DestroyWorld(m_PhysicsWorld);
	}

	void Scene::OnUpdateEditor(TimeStep ts, EditorCamera& camera)
	{
		Renderer2D::BeginScene(camera);

		auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
		for (auto entity : group)
		{
			auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

			Renderer2D::DrawSprite(transform.GetTransform(), sprite, (int)entity);
		}

		Renderer2D::EndScene();
	}

	void Scene::OnUpdateRuntime(TimeStep ts)
	{
		//更新脚本
		{
			m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc)
				{
					if (!nsc.Instance)
					{
						nsc.InstantiateScript(nsc.Instance);
						nsc.Instance->m_Entity = Entity{ entity, this };
						nsc.Instance->OnCreate();
					}
					nsc.Instance->OnUpdate(ts);
				});
		}

		//Physics
		{
			const int32_t velocityIterations = 6;
			const int32_t positionIterations = 2;

			b2World_Step(m_PhysicsWorld, ts, velocityIterations);
			//在Physics计算后更新transform
			auto view = m_Registry.view<Rigibody2DComponent>();
			for (auto e : view)
			{
				Entity entity = { e, this };
				auto& transform = entity.GetComponent<TransformComponent>();
				auto& rb2d = entity.GetComponent<Rigibody2DComponent>();

				const b2Vec2 position = b2Body_GetPosition(rb2d.RuntimeBody);
				transform.Translation.x = position.x;
				transform.Translation.y = position.y;

				b2Rot rot = b2Body_GetRotation(rb2d.RuntimeBody);
				transform.Rotation.z = b2Rot_GetAngle(rot);
			}
		}

		//Renderer 2D
		Camera* mainCamera = nullptr;
		glm::mat4* cameraTransform;

		auto views = m_Registry.view<TransformComponent, CameraComponent>();
		for (auto entity : views)
		{
			auto [transform, camera] = views.get<TransformComponent, CameraComponent>(entity);
			if (camera.Primary)
			{
				mainCamera = &camera.Camera;
				cameraTransform = &transform.GetTransform();
				break;
			}
		}

		if (mainCamera)
		{
			Renderer2D::BeginScene(mainCamera->GetProjection(), *cameraTransform);

			auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
			for (auto entity : group)
			{
				auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
				Renderer2D::DrawSprite(transform.GetTransform(), sprite, (int)entity);
			}

			Renderer2D::EndScene();
		}
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;

		//调整 非锁定纵横比的相机 的尺寸
		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			auto& cameraComponent = view.get<CameraComponent>(entity);
			if (!cameraComponent.FixedAspectRatio)
			{
				cameraComponent.Camera.SetViewportSize(width, height);
			}
		}
	}

	void Scene::DuplicateEntity(Entity entity)
	{
		std::string name = entity.GetName();
		Entity newEntity = CreateEntity(name);
		CopyComponentIfExists<TransformComponent>(newEntity, entity);
		CopyComponentIfExists<SpriteRendererComponent>(newEntity, entity);
		CopyComponentIfExists<CameraComponent>(newEntity, entity);
		CopyComponentIfExists<NativeScriptComponent>(newEntity, entity);
		CopyComponentIfExists<Rigibody2DComponent>(newEntity, entity);
		CopyComponentIfExists<BoxCollider2DComponent>(newEntity, entity);
	}

	Entity Scene::GetPrimaryCameraEntity()
	{
		auto view = m_Registry.view<CameraComponent>();
		for (auto ent :view)
		{
			const auto& camera = view.get<CameraComponent>(ent);
			if (camera.Primary)
			{
				return Entity{ent, this};
			}
		}
		return {};
	}
}

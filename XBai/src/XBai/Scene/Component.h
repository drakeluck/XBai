#ifndef XB_SCENE_COMPONENT_H
#define XB_SCENE_COMPONENT_H

#include "SceneCamera.h"
#include "XBai/Core/UUID.h"
#include "XBai/Render/Texture.h"
#include "../vendor/Box2D/include/box2d/box2d.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace XBai
{
	struct IDComponent
	{
		UUID ID;
		IDComponent() = default;
		IDComponent(const IDComponent&) = default;
	};

	struct TagComponent
	{
		std::string Tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag)
			:Tag(tag) {}
	};

	struct TransformComponent
	{
		glm::vec3 Translation = {0.0f, 0.0f, 0.0f};
		//这里存储的旋转单位是弧度，用户UI上再转为角度
		glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::vec3& translation)
			:Translation(translation) {}

		glm::mat4 GetTransform() const
		{
			glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));

			return glm::translate(glm::mat4(1.0f), Translation)
					* rotation
					* glm::scale(glm::mat4(1.0f), Scale);
		}
	};

	struct SpriteRendererComponent
	{
		glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f};
		Ref<Texture2D> Texture;
		float TilingFactor = 1.0f;

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec4& color)
			:Color(color) {}
	};

	struct CameraComponent
	{
		SceneCamera Camera;

		bool Primary = true;
		//FixedAspectRatio : 是否是固定纵横比
		bool FixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
	};

	class ScriptableEntity;
	struct NativeScriptComponent
	{
		ScriptableEntity* Instance = nullptr;

		void(*InstantiateScript)(ScriptableEntity*&) = nullptr;
		void(*DestroyScript)(ScriptableEntity*&) = nullptr;

		template<typename T>
		void Bind()
		{
			InstantiateScript = [](ScriptableEntity*& instance) { instance = new T(); };
			DestroyScript = [](ScriptableEntity*& instance) { delete static_cast<T*>(instance); instance = nullptr; };
		}
	};

	//刚体组件
	struct Rigibody2DComponent
	{
		//静态，运动体, 动态
		enum class BodyType { Static = 0, Kinematic, Dynamic };
	
		BodyType Type = BodyType::Static;
		//是否锁定旋转
		bool FixedRotation = false;
	
		b2BodyId RuntimeBody;

		Rigibody2DComponent() = default;
		Rigibody2DComponent(const Rigibody2DComponent&) = default;
	};

	//碰撞盒组件
	struct BoxCollider2DComponent
	{
		glm::vec2 Offset = { 0.0f, 0.0f };
		glm::vec2 Size = { 0.5f, 0.5f };

		//密度
		float Density = 1.0f;
		//摩擦力
		float Friction = 0.5f;
		//弹性
		float Restitution = 0.0f;
		//弹性阈值
		//float RestitutionThreshold = 0.5f;

		BoxCollider2DComponent() = default;
		BoxCollider2DComponent(const BoxCollider2DComponent&) = default;
	};

}

#endif

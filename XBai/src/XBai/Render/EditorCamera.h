﻿#ifndef XB_RENDER_EDITORCAMERA_H
#define XB_RENDER_EDITORCAMERA_H

#include "Camera.h"
#include "XBai/Core/TimeStep.h"
#include "XBai/Events/Event.h"
#include "XBai/Events/MouseEvent.h"

#include <glm/glm.hpp>

namespace XBai
{
	class EditorCamera : public Camera
	{
	public:
		EditorCamera() = default;
		EditorCamera(float fov, float aspectRatio, float nearClip, float farClip);

		void OnUpdate(TimeStep ts);
		void OnEvent(Event& e);

		inline float GetDistance() const { return m_Distance; }
		inline void SetDistance(float distance) { m_Distance = distance; }

		inline void SetViewportSize(float width, float height)
		{
			if (m_ViewportWidth != width || m_ViewportHeight != height)
			{
				m_ViewportWidth = width;
				m_ViewportHeight = height;
				UpdateProjection();
			}
		}

		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		glm::mat4 GetViewProjection() const { return m_Projection * m_ViewMatrix; }

		glm::vec3 GetUpDirection() const;
		glm::vec3 GetRightDirection() const;
		glm::vec3 GetForwardDirection() const;
		const glm::vec3& GetPosition() const { return m_Position; }
		glm::quat GetOrientation() const;

		float GetPitch() const { return m_Pitch; }
		float GetYaw() const { return m_Yaw; }

	private:
		void UpdateProjection();
		void UpdateView();

		bool OnMouseScroll(MouseScrolledEvent& e);

		void MousePan(const glm::vec2& delta);
		void MouseRotate(const glm::vec2& delta);
		void MouseZoom(float delta);

		glm::vec3 CalculatePosition() const;

		std::pair<float, float> PanSpeed() const;
		float RotationSpeed() const;
		float ZoomSpeed() const;
	private:
		float m_FOV = 45.0f;
		float m_AspectRatio = 1.778f;
		float m_NearClip = 0.1f;
		float m_FarClip = 1000.0f;

		glm::mat4 m_ViewMatrix = glm::mat4(1.0f);
		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		//焦点
		glm::vec3 m_FocalPoint = { 0.0f, 0.0f, 0.0f };//一定要初始化。。。血泪
		glm::vec2 m_InitialMousePosition = { 0.0f, 0.0f };//也一定要初始化。。。血泪

		float m_Distance = 10.0f;
		//俯仰角
		float m_Pitch = 0.0f;
		//偏航角
		float m_Yaw = 0.0f;

		float m_ViewportWidth = 1200;
		float m_ViewportHeight = 720;
	};
}

#endif


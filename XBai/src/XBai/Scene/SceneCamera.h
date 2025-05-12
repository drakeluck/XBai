#ifndef XB_SCENE_SCENE_CAMERA_H
#define XB_SCENE_SCENE_CAMERA_H

#include "XBai/Render/Camera.h"

#include <glm/glm.hpp>

namespace XBai
{
	/**
	 *场景摄像机
	 */
	class SceneCamera :public Camera
	{
	public:
		enum class ProjectionType { Perspective = 0, Orthographic = 1};

	public:
		SceneCamera();
		virtual ~SceneCamera() override = default;

		void SetProjectionType(ProjectionType type) { m_ProjectionType = type;  RecalculateProjection(); }
		ProjectionType GetProjectionType() const { return m_ProjectionType; }

		void SetViewportSize(uint32_t width, uint32_t height);

		/**
		 * 设置透视投影相机参数
		 * @param size 正交尺寸
		 * @param nearClip 近裁剪面
		 * @param farClip 远裁剪面
		 */
		void SetPerspective(float verticalFov, float nearClip, float farClip);

		void SetPerspectiveVerticalFOV(float verticalFov) { m_PerspectiveFOV = verticalFov; RecalculateProjection(); }
		float GetPerspectiveVerticalFOV() const { return m_PerspectiveFOV; }
		void SetPerspectiveNearClip(float nearClip) { m_PerspectiveNear = nearClip;  RecalculateProjection(); }
		float GetPerspectiveNearClip() const { return m_PerspectiveNear; }
		void SetPerspectiveFarClip(float farClip) { m_PerspectiveFar = farClip;  RecalculateProjection(); }
		float GetPerspectiveFarClip() const { return m_PerspectiveFar; }

		/**
		 * 设置正交投影相机参数
		 * @param size 正交尺寸
		 * @param nearClip 近裁剪面
		 * @param farClip 远裁剪面
		 */
		void SetOrthographic(float size, float nearClip, float farClip);

		void SetOrthographicSize(float size) { m_OrthographicSize = size; RecalculateProjection(); }
		float GetOrthographicSize() const { return m_OrthographicSize; }
		void SetOrthographicNearClip(float nearClip) { m_OrthographicNear = nearClip;  RecalculateProjection(); }
		float GetOrthographicNearClip() const { return m_OrthographicNear; }
		void SetOrthographicFarClip(float farClip) { m_OrthographicFar = farClip;  RecalculateProjection(); }
		float GetOrthographicFarClip() const { return m_OrthographicFar; }

	private:
		void RecalculateProjection();

	private:
		ProjectionType m_ProjectionType = ProjectionType::Orthographic;

		float m_PerspectiveFOV = glm::radians(45.0f);
		float m_PerspectiveNear = 0.01f;
		float m_PerspectiveFar = 100.0f;

		float m_OrthographicSize = 10.0f;
		float m_OrthographicNear = -1.0f;
		float m_OrthographicFar = 1.0f;

		float m_AspectRadio = 0.0f;
	};
}

#endif


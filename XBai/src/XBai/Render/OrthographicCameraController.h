#ifndef XB_ORTHOGRAPHIC_CAMERA_CONTROLLER_H
#define XB_ORTHOGRAPHIC_CAMERA_CONTROLLER_H

#include "XBai/Render/OrthographicCamera.h"
#include "XBai/Core/TimeStep.h"

#include "XBai/Events/ApplicationEvent.h"
#include "XBai/Events/MouseEvent.h"
#include "XBai/Events/KeyEvent.h"

namespace XBai
{
	struct OrthographicCameraBounds
	{
		float Left, Right;
		float Bottom, Top;

		float GetWidth() { return Right - Left; }
		float GetHeight() { return Top - Bottom; }
	};

	class OrthographicCameraController
	{
	public:
		//@param aspectRatio 纵横比
		OrthographicCameraController(float aspectRatio, bool rotation = false);
		~OrthographicCameraController();
		void OnUpdate(TimeStep ts);
		void OnEvent(Event& e);

		void OnResize(float width, float height);
		OrthographicCamera& GetCamera() { return m_Camera; }
		const OrthographicCamera& GetCamera() const { return m_Camera; }
		const OrthographicCameraBounds& GetBounds() { return m_Bounds; }
		void SetZoomLevel(float level) { m_ZoomLevel = level; CalculateView(); }
		float GetZoomLevel() const { return m_ZoomLevel; }
	private:
		void CalculateView();
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);
	private:
		//纵横比
		float m_AspectRatio;
		//缩放比
		float m_ZoomLevel = 1.0f;
		OrthographicCameraBounds m_Bounds;
		OrthographicCamera m_Camera;
		bool m_Rotation;

		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
		float m_CameraRotation = 0.0f;
		float m_CameraTranslateSpeed = 5.0f, m_CameraRotationSpeed = 45.0f;
	};
}

#endif


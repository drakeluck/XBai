#ifndef XB_ORTHOGRAPHIC_CAMERA_CONTROLLER_H
#define XB_ORTHOGRAPHIC_CAMERA_CONTROLLER_H

#include "XBai/Render/OrthographicCamera.h"
#include "XBai/Core/TimeStep.h"

#include "XBai/Events/ApplicationEvent.h"
#include "XBai/Events/MouseEvent.h"
#include "XBai/Events/KeyEvent.h"

namespace XBai
{
	class OrthographicCameraController
	{
	public:
		//@param aspectRatio 纵横比
		OrthographicCameraController(float aspectRatio, bool rotation = false);

		void OnUpdate(TimeStep ts);
		void OnEvent(Event& e);
		OrthographicCamera& GetCamera() { return m_Camera; }
		const OrthographicCamera& GetCamera() const { return m_Camera; }

		void SetZoomLevel(float level) { m_ZoomLevel = level; }
		float GetZoomLevel() const { return m_ZoomLevel; }
	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);
	private:
		//纵横比
		float m_AspectRatio;
		//缩放比
		float m_ZoomLevel = 1.0f;
		OrthographicCamera m_Camera;
		bool m_Rotation;

		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
		float m_CameraRotation = 0.0f;
		float m_CameraTranslateSpeed = 5.0f, m_CameraRotationSpeed = 45.0f;
	};
}

#endif


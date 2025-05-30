﻿#include "xbpch.h"
#include "OrthographicCameraController.h"

#include "XBai/Core/Input.h"
#include "XBai/Core/KeyCodes.h"

namespace XBai
{
	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
		:m_AspectRatio(aspectRatio), m_Bounds({ -m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel }),
		m_Camera(m_Bounds.Left, m_Bounds.Right, m_Bounds.Bottom, m_Bounds.Top),
		m_Rotation(rotation)
	{
	}

	OrthographicCameraController::~OrthographicCameraController()
	{
		
	}

	void OrthographicCameraController::OnUpdate(TimeStep ts)
	{
		XB_PROFILE_FUNCTION()

		if (Input::IsKeyPressed(XB_KEY_A))
			m_CameraPosition.x -= m_CameraTranslateSpeed * ts;
		else if (XBai::Input::IsKeyPressed(XB_KEY_D))
			m_CameraPosition.x += m_CameraTranslateSpeed * ts;

		if (Input::IsKeyPressed(XB_KEY_W))
			m_CameraPosition.y += m_CameraTranslateSpeed * ts;
		else if (Input::IsKeyPressed(XB_KEY_S))
			m_CameraPosition.y -= m_CameraTranslateSpeed * ts;
		if (m_Rotation)
		{
			if (Input::IsKeyPressed(XB_KEY_E))
				m_CameraRotation -= m_CameraRotationSpeed * ts;
			else if (Input::IsKeyPressed(XB_KEY_Q))
				m_CameraRotation += m_CameraRotationSpeed * ts;

			m_Camera.SetRotation(m_CameraRotation);
		}
		m_Camera.SetPosition(m_CameraPosition);
		m_CameraTranslateSpeed = m_ZoomLevel;
	}

	void OrthographicCameraController::OnEvent(Event& e)
	{
		XB_PROFILE_FUNCTION()

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(XB_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(XB_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
	}

	void OrthographicCameraController::OnResize(float width, float height)
	{
		m_AspectRatio = width / height;
		CalculateView();
	}

	void OrthographicCameraController::CalculateView()
	{
		m_Bounds = { -m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel };
		m_Camera.SetProjection(m_Bounds.Left, m_Bounds.Right, m_Bounds.Bottom, m_Bounds.Top);
	}

	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		XB_PROFILE_FUNCTION()

		m_ZoomLevel -= e.GetYOffset() * 0.25f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
		CalculateView();
		return false;
	}

	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e)
	{
		XB_PROFILE_FUNCTION()

		OnResize((float)e.GetWidth(), (float)e.GetHeight());
		// m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
		// CalculateView();
		return false;
	}

}

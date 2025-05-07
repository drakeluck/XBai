﻿#ifndef XB_RENDER_CAMERA_H
#define XB_RENDER_CAMERA_H

#include <glm/glm.hpp>

namespace XBai
{
	class Camera
	{
	public:
		Camera() = default;
		Camera(const glm::mat4& projection)
			:m_Projection(projection) {}
		virtual ~Camera() = default;

		const glm::mat4& GetProjection() const { return m_Projection; }
	protected:
		glm::mat4 m_Projection = glm::mat4(1.0f);
	};
}

#endif


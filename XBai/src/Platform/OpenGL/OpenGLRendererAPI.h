#ifndef XB_OPENGL_RENDERER_API_H
#define XB_OPENGL_RENDERER_API_H

#include "XBai/Render/RendererAPI.h"

namespace XBai
{
	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void Clear() override;

		virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) override;
	};
}

#endif

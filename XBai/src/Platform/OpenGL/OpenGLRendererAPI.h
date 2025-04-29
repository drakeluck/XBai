#ifndef XB_OPENGL_RENDERER_API_H
#define XB_OPENGL_RENDERER_API_H

#include "XBai/Render/RendererAPI.h"

namespace XBai
{
	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		virtual void Init() override;
		virtual void SetViewPort(uint32_t xOffset, uint32_t yOffset, uint32_t width, uint32_t height) override;
		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void Clear() override;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) override;
	};
}

#endif

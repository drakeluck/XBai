#ifndef XB_RENDER_COMMEND_H
#define XB_RENDER_COMMEND_H

#include "RendererAPI.h"

namespace XBai
{
	class RenderCommand
	{
	public:
		inline static void Init()
		{
			s_RendererAPI->Init();
		}

		inline static void SetViewPort(uint32_t xOffset, uint32_t yOffset, uint32_t width, uint32_t height)
		{
			s_RendererAPI->SetViewPort(xOffset, yOffset, width, height);
		}

		inline static void SetClearColor(const glm::vec4& color)
		{
			s_RendererAPI->SetClearColor(color);
		}

		inline static void Clear()
		{
			s_RendererAPI->Clear();
		}

		inline static void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0)
		{
			s_RendererAPI->DrawIndexed(vertexArray, indexCount);
		}

		inline static void DrawLines(const Ref<VertexArray>& vertexArray, uint32_t vertexCount)
		{
			s_RendererAPI->DrawLines(vertexArray, vertexCount);
		}

		inline static void SetLineWidth(float width)
		{
			s_RendererAPI->SetLineWidth(width);
		}

	private:
		static RendererAPI* s_RendererAPI;
	};

}

#endif

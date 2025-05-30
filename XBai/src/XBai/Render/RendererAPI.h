#ifndef XB_RENDERERAPI_H
#define XB_RENDERERAPI_H

#include <glm/glm.hpp>
#include "VertexArray.h"

namespace XBai
{
	class RendererAPI
	{
	public:
		enum class API
		{
			None = 0, OpenGL = 1
		};

	public:
		virtual void Init() = 0;
		virtual void SetViewPort(uint32_t xOffset, uint32_t yOffset, uint32_t width, uint32_t height) = 0;
		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear() = 0;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0) = 0;
		virtual void DrawLines(const Ref<VertexArray>& vertexArray, uint32_t vertexCount) = 0;

		virtual void SetLineWidth(float width) = 0;

		inline static API GetAPI() { return s_API; }
	private:
		static API s_API;
	};
}

#endif


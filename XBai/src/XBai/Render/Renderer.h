#ifndef XB_RENDERER_H
#define XB_RENDERER_H

#include "RenderCommend.h"

namespace XBai
{
	class Renderer
	{
	public:
		static void BeginScene();//TODO:
		static void EndScene();//TODO:
		static void Submit(const std::shared_ptr<VertexArray>& vertexArray);//TODO:

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	};
}

#endif

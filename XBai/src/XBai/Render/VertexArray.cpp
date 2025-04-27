#include "xbpch.h"
#include "VertexArray.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace XBai
{
	VertexArray* VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			XB_CORE_ASSERT(false, "RendererAPI::None is current not supported!")
			return nullptr;
		case RendererAPI::API::OpenGL:
			return new OpenGLVertexArray();
		}
		XB_CORE_ASSERT(false, "Unknow RendererAPI!")
		return nullptr;
	}
}

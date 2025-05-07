#include "xbpch.h"
#include "Buffer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"
#include "Renderer.h"

namespace XBai
{
	Ref<VertexBuffer> VertexBuffer::Create(uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			XB_CORE_ASSERT(false, "RendererAPI::None is current not supported!")
				return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLVertexBuffer>(size);
		}
		XB_CORE_ASSERT(false, "Unknow RendererAPI!")
			return nullptr;
	}

	Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			XB_CORE_ASSERT(false, "RendererAPI::None is current not supported!")
			return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLVertexBuffer>(vertices, size);
		}
		XB_CORE_ASSERT(false, "Unknow RendererAPI!")
		return nullptr;
	}

	

	Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			XB_CORE_ASSERT(false, "RendererAPI::None is current not supported!")
			return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLIndexBuffer>(indices, count);
		}
		XB_CORE_ASSERT(false, "Unknow RendererAPI!")
		return nullptr;
	}
}

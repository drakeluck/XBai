#include "xbpch.h"
#include "FrameBuffer.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLFrameBuffer.h"

namespace XBai
{
	Ref<FrameBuffer> FrameBuffer::Create(const FrameBufferSpecification& spec)
	{
		switch (Renderer::GetAPI())
		{

		case RendererAPI::API::None: XB_CORE_ASSERT(false, "RendererAPI::None is currently not supported!")  return nullptr;

		case RendererAPI::API::OpenGL: return CreateRef<OpenGLFrameBuffer>(spec);

		}

		XB_CORE_ASSERT(false, "Unknow Renderer API!")
		return nullptr;

	}
}

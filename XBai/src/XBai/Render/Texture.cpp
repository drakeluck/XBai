#include "xbpch.h"
#include "Texture.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace XBai
{
	Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: XB_CORE_ASSERT(false, "RendererAPI::None is currently not supported!")
		case RendererAPI::API::OpenGL:  return CreateRef<OpenGLTexture2D>(width, height);

		}
		XB_CORE_ASSERT(false, "Unknow RendererAPI!")
			return nullptr;
	}

	Ref<Texture2D> Texture2D::Create(const std::string& path)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: XB_CORE_ASSERT(false, "RendererAPI::None is currently not supported!")
		case RendererAPI::API::OpenGL:  return CreateRef<OpenGLTexture2D>(path);

		}
		XB_CORE_ASSERT(false, "Unknow RendererAPI!")
		return nullptr;
	}
}

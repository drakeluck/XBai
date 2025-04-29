#include "xbpch.h"
#include "Texture.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace XBai
{
	Ref<Texture2D> Texture2D::Create(const std::string& path)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: XB_CORE_ASSERT(false, "RendererAPI::None is currently not supported!")
		case RendererAPI::API::OpenGL:  return std::make_shared<OpenGLTexture2D>(path);

		}
		XB_CORE_ASSERT(false, "Unknow RendererAPI!")
		return nullptr;
	}
}

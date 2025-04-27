#include "xbpch.h"
#include "RenderCommend.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"
namespace XBai
{
	RendererAPI* RenderCommend::s_RendererAPI = new OpenGLRendererAPI;
}
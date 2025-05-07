#include "xbpch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"
namespace XBai
{
	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;
}
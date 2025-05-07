#include "xbpch.h"
#include "OpenGLRendererAPI.h"

#include <glad/glad.h>

namespace XBai
{
	void OpenGLRendererAPI::Init()
	{
		XB_PROFILE_FUNCTION()

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_DEPTH_TEST);
	}

	void OpenGLRendererAPI::SetViewPort(uint32_t xOffset, uint32_t yOffset, uint32_t width, uint32_t height)
	{
		glViewport(xOffset, yOffset, width, height);
	}

	void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRendererAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount)
	{
		glDrawElements(GL_TRIANGLES, indexCount == 0 ? vertexArray->GetIndexBuffer()->GetCount() : indexCount, GL_UNSIGNED_INT, nullptr);
		//glBindTexture(GL_TEXTURE_2D, 0);
	}
}

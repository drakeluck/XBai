#include "xbpch.h"
#include "Renderer.h"
#include "RenderCommend.h"

namespace XBai
{
	void Renderer::BeginScene()
	{
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(const std::shared_ptr<VertexArray>& vertexArray)
	{
		vertexArray->Bind();
		RenderCommend::DrawIndexed(vertexArray);
	}
}

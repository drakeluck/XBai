#include "xbpch.h"
#include "Renderer.h"
#include "RenderCommend.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace XBai
{
	Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData;

	void Renderer::Init()
	{
		RenderCommend::Init();
	}

	void XBai::Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		RenderCommend::SetViewPort(0, 0, width, height);
	}

	void Renderer::BeginScene(OrthographicCamera& camera)
	{
		m_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(
		const Ref<Shader>& shader, 
		const Ref<VertexArray>& vertexArray,
		const glm::mat4& transform)
	{
		shader->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_ViewProjection", m_SceneData->ViewProjectionMatrix);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_ModelMatrix", transform);

		vertexArray->Bind();
		RenderCommend::DrawIndexed(vertexArray);
	}
}

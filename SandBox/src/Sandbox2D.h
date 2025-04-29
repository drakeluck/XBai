#ifndef SANDBOX_SANDBOX2D_H
#define SANDBOX_SANDBOX2D_H
#include "XBai.h"

class Sandbox2D : public XBai::Layer
{

public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;
	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void OnUpdate(XBai::TimeStep timestep) override;
	virtual void OnImGuiRender()override;
	virtual void OnEvent(XBai::Event& e)override;


private:
	XBai::OrthographicCameraController m_CameraController;
	XBai::Ref<XBai::Shader> m_FlatShader;			// shader类 指针
	XBai::Ref<XBai::VertexArray> m_FlatVertexArray;

	glm::vec4 m_FlatColor = { 0.2f, 0.3f, 0.8f, 1.0f };
};

#endif


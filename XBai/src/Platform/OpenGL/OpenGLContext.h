#ifndef XB_OPENGL_CONTEXT_H
#define XB_OPENGL_CONTEXT_H

#include "XBai/Render/GraphicsContext.h"

struct GLFWwindow;

namespace XBai
{
	class OpenGLContext : public GraphicsContext
	{
	public:
		OpenGLContext(GLFWwindow* windowHandle);
		~OpenGLContext();

		virtual void Init() override;
		virtual void SwapBuffers() override;

	private:
		GLFWwindow* m_WindowHandle;
	};
}

#endif


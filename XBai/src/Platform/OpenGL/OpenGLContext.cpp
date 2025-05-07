#include "xbpch.h"
#include "OpenGLContext.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

XBai::OpenGLContext::OpenGLContext(GLFWwindow* windowHandle) :m_WindowHandle(windowHandle)
{
	XB_CORE_ASSERT(m_WindowHandle, "m_WindowHandle is null!")
}

XBai::OpenGLContext::~OpenGLContext()
{
}

void XBai::OpenGLContext::Init()
{
	XB_PROFILE_FUNCTION()

	glfwMakeContextCurrent(m_WindowHandle);
	int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	XB_CORE_ASSERT(status, "Failed to Init Glad!")

	XB_CORE_INFO("OpenGL Info:");
	XB_CORE_INFO("  Vendor : ({0})", (const char*)glGetString(GL_VENDOR));
	XB_CORE_INFO("  显卡名 : ({0})", (const char*)glGetString(GL_RENDERER));
	XB_CORE_INFO("  版本 : ({0})", (const char*)glGetString(GL_VERSION));

}

void XBai::OpenGLContext::SwapBuffers()
{
	XB_PROFILE_FUNCTION()
	glfwSwapBuffers(m_WindowHandle);
}

#ifndef XB_PLATFORM_OPENGLUNIFORMBUFFER_H
#define XB_PLATFORM_OPENGLUNIFORMBUFFER_H

#include "XBai/Render/Buffer.h"
namespace XBai
{
	class OpenGLUniformBuffer : public UniformBuffer
	{
	public:
		OpenGLUniformBuffer(uint32_t size, uint32_t binding);
		virtual ~OpenGLUniformBuffer() override;
		void SetData(const void* data, uint32_t size, uint32_t offset) override;
	private:
		uint32_t m_RendererID;
	};
}

#endif


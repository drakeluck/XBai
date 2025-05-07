#ifndef XB_RENDERER_FRAMEBUFFER_H
#define XB_RENDERER_FRAMEBUFFER_H

#include "XBai/Core/Core.h"
namespace XBai
{
	/**
	 * 帧缓冲区规范（明细单）
	 * @param Width 
	 * @param Height 
	 * @param Samples 样本数，可能是采样数
	 * @param SwapChainTarget 交换链目标
	 */
	struct FrameBufferSpecification
	{
		uint32_t Width, Height;
		uint32_t Samples = 1;

		bool SwapChainTarget = false;

		FrameBufferSpecification(uint32_t width = 1280, uint32_t height = 720)
			:Width(width), Height(height)
		{}
	};


	class FrameBuffer
	{
	public:

		virtual ~FrameBuffer() = default;
		//TODO
		//virtual FrameBufferSpecification& GetSpecification() = 0;
		virtual const FrameBufferSpecification& GetSpecification() const = 0;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual void Resize(uint32_t width, uint32_t height) = 0;

		virtual uint32_t GetColorAttachmentRendererID() const = 0;

		static Ref<FrameBuffer> Create(const FrameBufferSpecification& spec);

	};
}

#endif


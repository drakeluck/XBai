#ifndef XB_RENDERER_FRAMEBUFFER_H
#define XB_RENDERER_FRAMEBUFFER_H

#include "XBai/Core/Core.h"
namespace XBai
{
	//帧缓冲纹理格式
	enum class FramebufferTextureFormat
	{
		None = 0,
		//Color
		RGBA8, RED_INTEGER,

		//Depth/stencil
		DEPTH24STENCIL8,

		//Defaults
		Depth = DEPTH24STENCIL8
	};

	//帧缓冲纹理规范
	struct FramebufferTextureSpecification
	{
		FramebufferTextureSpecification() = default;
		FramebufferTextureSpecification(FramebufferTextureFormat format)
			:TextureFormat(format) {}

		FramebufferTextureFormat TextureFormat = FramebufferTextureFormat::None;
	};

	//帧缓冲附件规范
	struct FramebufferAttachmentSpecification
	{
		FramebufferAttachmentSpecification() = default;
		FramebufferAttachmentSpecification(std::initializer_list<FramebufferTextureSpecification> attachments)
			:Attachments(attachments){}

		std::vector<FramebufferTextureSpecification> Attachments;
	};

	/**
	 * 帧缓冲区规范（明细单）
	 * @param Width 
	 * @param Height 
	 * @param Samples 样本数，可能是采样数
	 * @param SwapChainTarget 交换链目标
	 * @param Attachments 帧缓冲附件规范
	 */
	struct FrameBufferSpecification
	{
		uint32_t Width, Height;
		uint32_t Samples = 1;

		bool SwapChainTarget = false;

		FramebufferAttachmentSpecification Attachments;

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

		virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) = 0;

		virtual void ClearAttachment(uint32_t attachmentIndex, int value) = 0;

		virtual uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const = 0;

		static Ref<FrameBuffer> Create(const FrameBufferSpecification& spec);

	};
}

#endif


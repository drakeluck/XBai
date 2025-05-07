#ifndef XB_TEXTURE_H
#define XB_TEXTURE_H

#include <string>
#include "XBai/Core/Core.h"

namespace XBai
{
	class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual uint32_t GetRendererID() const = 0;
		//向纹理内存中填充数据
		virtual void SetData(void* data, uint32_t size) = 0;

		virtual void Bind(uint32_t slot = 0) const = 0;

		virtual bool operator==(const Texture& other) const = 0;
	};

	class Texture2D :public Texture
	{
	public:
		//加载路径为path的文件作为纹理。
		static Ref<Texture2D> Create(const std::string& path);
		//只开辟一块纹理内存并设置一些纹理属性，需搭配SetData()方法来填充数据。
		static Ref<Texture2D> Create(uint32_t width, uint32_t height);
	};

}

#endif

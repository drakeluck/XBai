﻿#include "xbpch.h"
#include "OpenGLTexture.h"
#include "stb_image.h"

#include <glad/glad.h>

namespace XBai
{
	OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height)
		:m_Width(width), m_Height(height)
	{
		XB_PROFILE_FUNCTION()

		m_InternalFormat = GL_RGBA8;
		m_DataFormat = GL_RGBA;
		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
		:m_Path(path)
	{
		XB_PROFILE_FUNCTION()

		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = nullptr;
		{
			XB_PROFILE_SCOPE("stbi_load  in OpenGLTexture2D::OpenGLTexture2D(const std::string& path)")

			//char通常占一个字节，范围（-128 ~ 127）， 或无符号的（0 ~ 255）
			data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		}
		
		XB_CORE_ASSERT(data, "Failed to load image!?")
		m_Width = width;
		m_Height = height;

		if (channels == 4)
		{
			m_InternalFormat = GL_RGBA8;
			m_DataFormat = GL_RGBA;
		}
		else if (channels == 3)
		{
			m_InternalFormat = GL_RGBA8;
			m_DataFormat = GL_RGB;
		}

		//XB_CORE_ASSERT(m_InternalFormat & m_DataFormat, "Image data's format is not supported!")

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
		stbi_image_free(data);
	}

	void OpenGLTexture2D::SetData(void* data, uint32_t size)
	{
		uint32_t bpp = m_DataFormat == GL_RGBA ? 4 : 3;
		XB_CORE_ASSERT(size == m_Width * m_Height * bpp, "Size is wrong.")
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		XB_PROFILE_FUNCTION()

		glDeleteTextures(1, &m_RendererID);
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
		XB_PROFILE_FUNCTION()

		glBindTextureUnit(slot, m_RendererID);
	}

}

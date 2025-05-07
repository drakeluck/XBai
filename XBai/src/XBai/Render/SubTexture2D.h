#ifndef XB_SUBTEXTURE2D_H
#define XB_SUBTEXTURE2D_H

#include "Texture.h"

#include <glm/glm.hpp>

namespace XBai
{
	class SubTexture2D 
	{
	public:
		SubTexture2D(const Ref<Texture2D>& texture,const glm::vec2& min, const glm::vec2& max);
		const Ref<Texture2D> GetTexture() const { return m_Texture; }
		const glm::vec2* GetTexCoords() const { return m_TexCoords; }
		/**
		 * @param texture 精灵纹理图集
		 * @param coords 子精灵纹理在纹理图集中的位置，以cellsize为单位
		 * @param cellSize 最小子精灵的尺寸，以像素为单位
		 * @param spriteSize 该子精灵的尺寸，以cellsize为单位
		 */
		static Ref<SubTexture2D> CreateFromCoords(const Ref<Texture2D>& texture, const glm::vec2& coords, const glm::vec2& cellSize, const glm::vec2& spriteSize = { 1, 1 });
	private:
		Ref<Texture2D> m_Texture;
		glm::vec2 m_TexCoords[4];

	};
}

#endif



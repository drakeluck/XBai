#ifndef XB_RENDERER2D_H
#define XB_RENDERER2D_H

#include "Camera.h"
#include "OrthographicCamera.h"
#include "Texture.h"
#include "SubTexture2D.h"

namespace XBai
{
	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();
		static void BeginScene(const OrthographicCamera& camera);//TODO: will remove
		static void BeginScene(const Camera& camera, const glm::mat4& transform);
		static void Flush();
		static void EndScene();

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D> texture, float tilingFactor = 1.0f);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D> texture, float tilingFactor = 1.0f);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<SubTexture2D> subTexture, float tilingFactor = 1.0f);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D> subTexture, float tilingFactor = 1.0f);

		static void DrawQuad(const glm::mat4& transform, const glm::vec4& color);
		static void DrawQuad(const glm::mat4& transform, const Ref<Texture2D> texture, float tilingFactor = 1.0f, const glm::vec4& tineColor = glm::vec4(1.0f));

		static void DrawRotatedQuad(
				const glm::vec2& position,									//矩形绘制位置(以矩形左下角为起始)（2D位置）
				const glm::vec2& size, 										//矩形尺寸
				float rotation, 											//矩形旋转角度
				const glm::vec4& color										//矩形填充颜色
			);
		static void DrawRotatedQuad(
				const glm::vec3& position,									//矩形绘制位置(以矩形左下角为起始)（3D位置）
				const glm::vec2& size, 										//矩形尺寸
				float rotation, 											//矩形旋转角度
				const glm::vec4& color										//矩形填充颜色
			);
		static void DrawRotatedQuad(
				const glm::vec2& position,									//矩形绘制位置(以矩形左下角为起始)（2D位置）
				const glm::vec2& size, 										//矩形尺寸
				float rotation, 											//矩形旋转角度
				const Ref<Texture2D> texture, 								//矩形里的纹理
				float tillingFactor = 1.0f 									//纹理的平铺因子
			);
		static void DrawRotatedQuad(
				const glm::vec3& position,									//矩形绘制位置(以矩形左下角为起始)（3D位置）
				const glm::vec2& size, 										//矩形尺寸
				float rotation, 											//矩形旋转角度
				const Ref<Texture2D> texture, 								//矩形里的纹理
				float tillingFactor = 1.0f									//纹理的平铺因子
			);

		struct Statistics
		{
			uint32_t DrawCalls = 0;
			uint32_t QuadCount = 0;
			uint32_t GetTotalVertexCount() { return QuadCount * 4; }
			uint32_t GetTotalIndexCount() { return QuadCount * 6; }

		};
		static void ResetStats();
		static Statistics GetStats();

	private:
		static void FlushAndReset();
	};
}

#endif


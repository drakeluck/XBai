#ifndef XB_SHADER_H
#define XB_SHADER_H

#include <string>

namespace XBai
{
	class Shader
	{
	public: 
		Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
		~Shader();

		void Bind() const;
		void Unbind() const;

	private:
		uint32_t m_RendererID;
	};

}

#endif

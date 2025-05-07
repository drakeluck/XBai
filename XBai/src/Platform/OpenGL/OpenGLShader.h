﻿#ifndef XB_OPENGL_SHADER_H
#define XB_OPENGL_SHADER_H

#include "XBai/Render/Shader.h"
#include <glm/glm.hpp>

namespace XBai
{
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& filePath);
		OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
		virtual ~OpenGLShader() override;

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual const std::string& GetName() const override { return m_Name; }

		virtual void SetInt(const std::string& name, int value) override;
		virtual void SetIntArray(const std::string& name, int* value, uint32_t count) override;
		virtual void SetFloat(const std::string& name, float value) override;
		virtual void SetFloat3(const std::string& name, const glm::vec3 value) override;
		virtual void SetFloat4(const std::string& name, const glm::vec4 value) override;
		virtual void SetMat4(const std::string& name, const glm::mat4 value)   override;

		void UploadUniformMat3(const std::string& name, const glm::mat3& matrix);
		void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);

		void UploadUniformIntArray(const std::string& name, int* value, uint32_t count);
		void UploadUniformInt(const std::string& name, uint32_t values);
		void UploadUniformInt2(const std::string& name, const glm::ivec2& values);
		void UploadUniformInt3(const std::string& name, const glm::ivec3& values);
		void UploadUniformInt4(const std::string& name, const glm::ivec4& values);

		void UploadUniformFloat(const std::string& name, float values);
		void UploadUniformFloat2(const std::string& name, const glm::vec2& values);
		void UploadUniformFloat3(const std::string& name, const glm::vec3& values);
		void UploadUniformFloat4(const std::string& name, const glm::vec4& values);

	private:
		std::string ReadFile(const std::string& filePath);
		std::unordered_map<unsigned int, std::string> PreProcess(const std::string& source);
		void Compile(const std::unordered_map<unsigned int, std::string>& shaderSources);

	private:
		uint32_t m_RendererID;
		std::string m_Name;
	};
}

#endif

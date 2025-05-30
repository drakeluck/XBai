﻿#include "xbpch.h"
#include "OpenGLShader.h"
#include "XBai/Core/Timer.h"

#include <filesystem>
#include <fstream>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <shaderc/shaderc.hpp>
#include <spirv_cross/spirv_cross.hpp>
#include <spirv_cross/spirv_glsl.hpp>

namespace XBai
{
	namespace Utils
	{

		static GLenum ShaderTypeFromString(const std::string& type)
		{
			if (type == "vertex")
				return GL_VERTEX_SHADER;
			if (type == "fragment" || type == "pixel")
				return GL_FRAGMENT_SHADER;

			XB_CORE_ASSERT(false, "Unknown shader type!")
			return 0;
		}

		static shaderc_shader_kind GLShaderStageToShaderC(GLenum stage)
		{
			switch (stage)
			{
			case GL_VERTEX_SHADER:   return shaderc_glsl_vertex_shader;
			case GL_FRAGMENT_SHADER: return shaderc_glsl_fragment_shader;
			}
			XB_CORE_ASSERT(false, "Unsupported Shader");
			return (shaderc_shader_kind)0;
		}

		static const char* GLShaderStageToString(GLenum stage)
		{
			switch (stage)
			{
			case GL_VERTEX_SHADER:   return "GL_VERTEX_SHADER";
			case GL_FRAGMENT_SHADER: return "GL_FRAGMENT_SHADER";
			}
			XB_CORE_ASSERT(false, "Unsupported Shader");
			return nullptr;
		}

		static const char* GetCacheDirectory()
		{
			// TODO: make sure the assets directory is valid
			return "assets/cache/shader/opengl";
		}

		static void CreateCacheDirectoryIfNeeded()
		{
			std::string cacheDirectory = GetCacheDirectory();
			if (!std::filesystem::exists(cacheDirectory))
				std::filesystem::create_directories(cacheDirectory);
		}

		static const char* GLShaderStageCachedOpenGLFileExtension(uint32_t stage)
		{
			switch (stage)
			{
			case GL_VERTEX_SHADER:    return ".cached_opengl.vert";
			case GL_FRAGMENT_SHADER:  return ".cached_opengl.frag";
			}
			XB_CORE_ASSERT(false, "Unsupported Shader");
			return "";
		}

		static const char* GLShaderStageCachedVulkanFileExtension(uint32_t stage)
		{
			switch (stage)
			{
			case GL_VERTEX_SHADER:    return ".cached_vulkan.vert";
			case GL_FRAGMENT_SHADER:  return ".cached_vulkan.frag";
			}
			XB_CORE_ASSERT(false, "Unsupported Shader");
			return "";
		}
	}

	static GLenum ShaderTypeFromString(std::string& type)
	{
		if (type == "vertex")
			return GL_VERTEX_SHADER;
		else if (type == "fragment" || type == "pixel")
			return GL_FRAGMENT_SHADER;
		//TODO:未来添加其他着色器类型
		XB_CORE_ASSERT(false, "UnSupported ShaderType.")
		return 0;
	}

	OpenGLShader::OpenGLShader(const std::string& filepath)
		:m_FilePath(filepath)
	{
		XB_PROFILE_FUNCTION()

		Utils::CreateCacheDirectoryIfNeeded();

		std::string source = ReadFile(filepath);
		auto shaderSources = PreProcess(source);
		{
			Timer timer;
			CompileOrGetVulkanBinaries(shaderSources);
			CompileOrGetOpenGLBinaries();
			CreateProgram();
			XB_CORE_WARN("Shader creation took {0} ms", timer.ElapsedMillis());
		}
		// Extract name from filepath
		auto lastSlash = filepath.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = filepath.rfind('.');
		auto count = lastDot == std::string::npos ? filepath.size() - lastSlash : lastDot - lastSlash;
		m_Name = filepath.substr(lastSlash, count);

		// XB_PROFILE_FUNCTION()
		//
		// std::string source = ReadFile(filePath);
		// auto shadersource = PreProcess(source);
		// Compile(shadersource);
		// auto lastSlash = filePath.find_last_of("/\\");
		// lastSlash = lastSlash == std::string::npos ? 0 : (lastSlash + 1);
		//
		// auto lastDot = filePath.find_first_of(".");
		// auto count = lastDot == std::string::npos ? filePath.size() - lastSlash : lastDot - lastSlash;
		// m_Name = filePath.substr(lastSlash, count);
	}

	OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
		:m_Name(name)
	{
		XB_PROFILE_FUNCTION()

		std::unordered_map<GLenum, std::string> sources;
		sources[GL_VERTEX_SHADER] = vertexSrc;
		sources[GL_FRAGMENT_SHADER] = fragmentSrc;

		CompileOrGetVulkanBinaries(sources);
		CompileOrGetOpenGLBinaries();
		CreateProgram();

		// XB_PROFILE_FUNCTION()
		//
		// std::unordered_map<unsigned int, std::string> sources;
		// sources[GL_VERTEX_SHADER] = vertexSrc;
		// sources[GL_FRAGMENT_SHADER] = fragmentSrc;
		// Compile(sources);
	}
	OpenGLShader::~OpenGLShader()
	{
		XB_PROFILE_FUNCTION()

		glDeleteProgram(m_RendererID);
	}

	void OpenGLShader::Bind() const
	{
		XB_PROFILE_FUNCTION()

		glUseProgram(m_RendererID);
	}

	void OpenGLShader::Unbind() const
	{
		XB_PROFILE_FUNCTION()

		glUseProgram(0);
	}

	void OpenGLShader::SetInt(const std::string& name, int value)
	{
		XB_PROFILE_FUNCTION()

		UploadUniformInt(name, value);
	}

	void OpenGLShader::SetIntArray(const std::string& name, int* value, uint32_t count)
	{
		UploadUniformIntArray(name, value, count);
	}

	void OpenGLShader::SetFloat(const std::string& name, float value)
	{
		XB_PROFILE_FUNCTION()

		UploadUniformFloat(name, value);
	}

	void OpenGLShader::SetFloat3(const std::string& name, const glm::vec3 value)
	{
		XB_PROFILE_FUNCTION()

		UploadUniformFloat3(name, value);
	}

	void OpenGLShader::SetFloat4(const std::string& name, const glm::vec4 value)
	{
		XB_PROFILE_FUNCTION()

		UploadUniformFloat4(name, value);
	}

	void OpenGLShader::SetMat4(const std::string& name, const glm::mat4 value)
	{
		XB_PROFILE_FUNCTION()

		UploadUniformMat4(name, value);
	}

	void OpenGLShader::UploadUniformMat3(const std::string& name, const glm::mat3& matrix)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::UploadUniformIntArray(const std::string& name, int* value, uint32_t count)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1iv(location, count, value);
		//glUniform1uiv(location, count, value);
	}

	void OpenGLShader::UploadUniformInt(const std::string& name, uint32_t value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1i(location, value);
	}

	void OpenGLShader::UploadUniformInt2(const std::string& name, const glm::ivec2& values)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform2i(location, values.x, values.y);
	}

	void OpenGLShader::UploadUniformInt3(const std::string& name, const glm::ivec3& values)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform3i(location, values.x, values.y, values.z);
	}

	void OpenGLShader::UploadUniformInt4(const std::string& name, const glm::ivec4& values)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform4i(location, values.x, values.y, values.z, values.w);
	}

	void OpenGLShader::UploadUniformFloat(const std::string& name, float value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1f(location, value);
	}

	void OpenGLShader::UploadUniformFloat2(const std::string& name, const glm::vec2& values)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform2f(location, values.x, values.y);
	}

	void OpenGLShader::UploadUniformFloat3(const std::string& name, const glm::vec3& values)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform3f(location, values.x, values.y, values.z);
	}

	void OpenGLShader::UploadUniformFloat4(const std::string& name, const glm::vec4& values)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform4f(location, values.x, values.y, values.z, values.w);
	}

	std::string OpenGLShader::ReadFile(const std::string& filepath)
	{
		XB_PROFILE_FUNCTION()

		std::string result;
		std::ifstream in(filepath, std::ios::in | std::ios::binary);
		if (in)
		{
			in.seekg(0, std::ios::end);
			size_t size = in.tellg();
			if (size != -1)
			{
				result.resize(size);
				in.seekg(0, std::ios::beg);
				in.read(&result[0], size);
				//in.close();
			}
			else
			{
				XB_CORE_ERROR("Could not read from file '{0}'", filepath);
			}
			// result.resize(in.tellg());
			// in.seekg(0, std::ios::beg);
			// in.read(&result[0], result.size());
			// in.close();
		}
		else
		{
			XB_CORE_ERROR("Could not open file : {0}", filepath);
		}
		return result;
	}

	std::unordered_map<unsigned int, std::string> OpenGLShader::PreProcess(const std::string& source)
	{
		XB_PROFILE_FUNCTION()

		std::unordered_map<unsigned int, std::string> shaderSources;

		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0);
		while (pos != std::string::npos)
		{
			size_t eol = source.find_first_of("\r\n", pos);
			XB_CORE_ASSERT(eol != std::string::npos, "Syntax error")
			size_t begin = pos + typeTokenLength + 1;
			std::string type = source.substr(begin, eol - begin);
			XB_CORE_ASSERT(type == "vertex" || type == "fragment" || type == "pixel", "Invalid shader type specified")

			size_t nextLinePos = source.find_first_not_of("\r\n", eol);
			pos = source.find(typeToken, nextLinePos);
			shaderSources[ShaderTypeFromString(type)] = 
				source.substr(nextLinePos,
					pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos));
			
		}
		return shaderSources;
	}

	void OpenGLShader::CompileOrGetVulkanBinaries(const std::unordered_map<GLenum, std::string>& shaderSources)
	{
		GLuint program = glCreateProgram();

		shaderc::Compiler compiler;
		shaderc::CompileOptions options;
		options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_2);
		const bool optimize = true;
		if (optimize)
			options.SetOptimizationLevel(shaderc_optimization_level_performance);

		std::filesystem::path cacheDirectory = Utils::GetCacheDirectory();

		auto& shaderData = m_VulkanSPIRV;
		shaderData.clear();
		for (auto&& [stage, source] : shaderSources)
		{
			std::filesystem::path shaderFilePath = m_FilePath;
			std::filesystem::path cachedPath = cacheDirectory / (shaderFilePath.filename().string() + Utils::GLShaderStageCachedVulkanFileExtension(stage));

			std::ifstream in(cachedPath, std::ios::in | std::ios::binary);
			if (in.is_open())
			{
				in.seekg(0, std::ios::end);
				auto size = in.tellg();
				in.seekg(0, std::ios::beg);

				auto& data = shaderData[stage];
				data.resize(size / sizeof(uint32_t));
				in.read((char*)data.data(), size);
			}
			else
			{
				shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(source, Utils::GLShaderStageToShaderC(stage), m_FilePath.c_str(), options);
				if (module.GetCompilationStatus() != shaderc_compilation_status_success)
				{
					XB_CORE_ERROR(module.GetErrorMessage());
					XB_CORE_ASSERT(false, "失败");
				}

				shaderData[stage] = std::vector<uint32_t>(module.cbegin(), module.cend());

				std::ofstream out(cachedPath, std::ios::out | std::ios::binary);
				if (out.is_open())
				{
					auto& data = shaderData[stage];
					out.write((char*)data.data(), data.size() * sizeof(uint32_t));
					out.flush();
					out.close();
				}
			}
		}

		for (auto&& [stage, data] : shaderData)
			Reflect(stage, data);
	}

	void OpenGLShader::CompileOrGetOpenGLBinaries()
	{
		auto& shaderData = m_OpenGLSPIRV;

		shaderc::Compiler compiler;
		shaderc::CompileOptions options;
		options.SetTargetEnvironment(shaderc_target_env_opengl, shaderc_env_version_opengl_4_5);
		const bool optimize = false;
		if (optimize)
			options.SetOptimizationLevel(shaderc_optimization_level_performance);

		std::filesystem::path cacheDirectory = Utils::GetCacheDirectory();

		shaderData.clear();
		m_OpenGLSourceCode.clear();
		for (auto&& [stage, spirv] : m_VulkanSPIRV)
		{
			std::filesystem::path shaderFilePath = m_FilePath;
			std::filesystem::path cachedPath = cacheDirectory / (shaderFilePath.filename().string() + Utils::GLShaderStageCachedOpenGLFileExtension(stage));

			std::ifstream in(cachedPath, std::ios::in | std::ios::binary);
			if (in.is_open())
			{
				in.seekg(0, std::ios::end);
				auto size = in.tellg();
				in.seekg(0, std::ios::beg);

				auto& data = shaderData[stage];
				data.resize(size / sizeof(uint32_t));
				in.read((char*)data.data(), size);
				//in.close();
			}
			else
			{
				spirv_cross::CompilerGLSL glslCompiler(spirv);
				m_OpenGLSourceCode[stage] = glslCompiler.compile();
				auto& source = m_OpenGLSourceCode[stage];

				shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(source, Utils::GLShaderStageToShaderC(stage), m_FilePath.c_str());
				if (module.GetCompilationStatus() != shaderc_compilation_status_success)
				{
					XB_CORE_ERROR(module.GetErrorMessage());
					XB_CORE_ASSERT(false, "失败");
				}

				shaderData[stage] = std::vector<uint32_t>(module.cbegin(), module.cend());

				std::ofstream out(cachedPath, std::ios::out | std::ios::binary);
				if (out.is_open())
				{
					auto& data = shaderData[stage];
					out.write((char*)data.data(), data.size() * sizeof(uint32_t));
					out.flush();
					out.close();
				}
			}
		}
	}

	void OpenGLShader::CreateProgram()
	{
		GLuint program = glCreateProgram();

		std::vector<GLuint> shaderIDs;
		for (auto&& [stage, spirv] : m_OpenGLSPIRV)
		{
			GLuint shaderID = shaderIDs.emplace_back(glCreateShader(stage));
			glShaderBinary(1, &shaderID, GL_SHADER_BINARY_FORMAT_SPIR_V, spirv.data(), spirv.size() * sizeof(uint32_t));
			glSpecializeShader(shaderID, "main", 0, nullptr, nullptr);
			glAttachShader(program, shaderID);
		}

		glLinkProgram(program);

		GLint isLinked;
		glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, infoLog.data());
			XB_CORE_ERROR("Shader linking failed ({0}):\n{1}", m_FilePath, infoLog.data());

			glDeleteProgram(program);

			for (auto id : shaderIDs)
				glDeleteShader(id);
		}

		for (auto id : shaderIDs)
		{
			glDetachShader(program, id);
			glDeleteShader(id);
		}

		m_RendererID = program;
	}

	void OpenGLShader::Reflect(GLenum stage, const std::vector<uint32_t>& shaderData)
	{
		spirv_cross::Compiler compiler(shaderData);
		spirv_cross::ShaderResources resources = compiler.get_shader_resources();

		XB_CORE_TRACE("OpenGLShader::Reflect - {0} {1}", Utils::GLShaderStageToString(stage), m_FilePath);
		XB_CORE_TRACE("    {0} uniform buffers", resources.uniform_buffers.size());
		XB_CORE_TRACE("    {0} resources", resources.sampled_images.size());

		XB_CORE_TRACE("Uniform buffers:");
		for (const auto& resource : resources.uniform_buffers)
		{
			const auto& bufferType = compiler.get_type(resource.base_type_id);
			uint32_t bufferSize = compiler.get_declared_struct_size(bufferType);
			uint32_t binding = compiler.get_decoration(resource.id, spv::DecorationBinding);
			int memberCount = bufferType.member_types.size();

			XB_CORE_TRACE("  {0}", resource.name);
			XB_CORE_TRACE("    Size = {0}", bufferSize);
			XB_CORE_TRACE("    Binding = {0}", binding);
			XB_CORE_TRACE("    Members = {0}", memberCount);
		}
	}

	// void OpenGLShader::Compile(const std::unordered_map<unsigned int, std::string>& shaderSources)
	// {
	// 	XB_PROFILE_FUNCTION()
	//
	// 	unsigned int program = glCreateProgram();
	// 	std::array<GLenum, 2> glShaderIDs;
	// 	int glShaderIDIndex = 0;
	// 	for (auto& kv : shaderSources)
	// 	{
	// 		unsigned int type = kv.first;
	// 		const std::string& source = kv.second;
	//
	// 		// 1.1.创建着色器对象
	// 		unsigned int shader = glCreateShader(type);
	// 		const char* sourceChar = source.c_str();
	// 		glShaderSource(shader, 1, &sourceChar, 0);
	// 		glCompileShader(shader);
	// 		int isCompiled = 0;
	// 		glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
	// 		if (isCompiled == GL_FALSE) {
	// 			// 1.4.2编译失败可以打印报错信息
	// 			int maxLength = 0;
	// 			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
	// 			// The maxLength includes the NULL character
	// 			std::vector<GLchar> infoLog(maxLength);
	// 			glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);
	// 			// We don't need the shader anymore.
	// 			glDeleteShader(shader);
	// 			XB_CORE_ERROR("{0}", infoLog.data());
	// 			XB_CORE_ASSERT(false, "Shader compilation failure!")
	// 			return;
	// 		}
	// 		glAttachShader(program, shader);
	// 		//glShaderIDs.push_back(shader);
	// 		glShaderIDs[glShaderIDIndex] = shader;
	// 		glShaderIDIndex++;
	// 	}
	// 	glLinkProgram(program);
	// 	int isLinked = 0;
	// 	glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
	// 	if (isLinked == GL_FALSE) {
	// 		int maxLength = 0;
	// 		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
	// 		// The maxLength includes the NULL character
	// 		std::vector<char> infoLog(maxLength);
	// 		glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);
	// 		// We don't need the program anymore.
	// 		glDeleteProgram(program);
	// 		// Don't leak shaders either.
	// 		for (auto shaderIDs : glShaderIDs)
	// 		{
	// 			glDeleteShader(shaderIDs);
	// 		}
	// 		XB_CORE_ERROR("{0}", infoLog.data());
	// 		XB_CORE_ASSERT(false, "Shader link failure!")
	// 		return;
	// 	}
	// 	
	// 	for (auto shaderIDs : glShaderIDs)
	// 	{
	// 		glDetachShader(program, shaderIDs);
	// 	}
	// 	m_RendererID = program;
	// }
}

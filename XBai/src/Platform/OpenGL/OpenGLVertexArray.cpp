﻿#include "xbpch.h"
#include "OpenGLVertexArray.h"
#include "glad/glad.h"

namespace XBai
{
	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
			case ShaderDataType::Float:		return GL_FLOAT;
			case ShaderDataType::Float2:	return GL_FLOAT;
			case ShaderDataType::Float3:	return GL_FLOAT;
			case ShaderDataType::Float4:	return GL_FLOAT;
			case ShaderDataType::Mat3:		return GL_FLOAT;
			case ShaderDataType::Mat4:		return GL_FLOAT;
			case ShaderDataType::Int:		return GL_INT;
			case ShaderDataType::Int2:		return GL_INT;
			case ShaderDataType::Int3:		return GL_INT;
			case ShaderDataType::Int4:		return GL_INT;
			case ShaderDataType::Bool:		return GL_BOOL;
		}
		XB_CORE_ASSERT(false, "Unknow ShaderDataType!")
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		XB_PROFILE_FUNCTION()

		glCreateVertexArrays(1, &m_RendererID);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		XB_PROFILE_FUNCTION()

		glDeleteVertexArrays(1, &m_RendererID);
	}

	void OpenGLVertexArray::Bind() const
	{
		XB_PROFILE_FUNCTION()

		glBindVertexArray(m_RendererID);
	}

	void OpenGLVertexArray::Unbind() const
	{
		XB_PROFILE_FUNCTION()

		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
	{
		XB_PROFILE_FUNCTION()

		XB_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex buffer has no layout!")
		glBindVertexArray(m_RendererID);
		vertexBuffer->Bind();
		
		const auto& layout = vertexBuffer->GetLayout();
		for (const auto& element : layout)
		{
			switch(element.Type)
			{
				case ShaderDataType::Float:
				case ShaderDataType::Float2:
				case ShaderDataType::Float3:
				case ShaderDataType::Float4:
				{
					glEnableVertexAttribArray(m_VertexBufferIndex);
					glVertexAttribPointer(m_VertexBufferIndex, //layout(position = ${m_VertexBufferIndex})
						element.GetComponentCount(), //该组数据的数据单元个数
						ShaderDataTypeToOpenGLBaseType(element.Type),//该组数据的数据单元的类型
						element.Normalized ? GL_TRUE : GL_FALSE, //数据是否要归一化
						layout.GetStride(),	 //该顶点数据的步长
						(const void*)element.Offset);//该组数据在顶点数据中的起始偏移量
					m_VertexBufferIndex++;
					break;
				}
				case ShaderDataType::Int:
				case ShaderDataType::Int2:
				case ShaderDataType::Int3:
				case ShaderDataType::Int4:
				case ShaderDataType::Bool:
				{
					glEnableVertexAttribArray(m_VertexBufferIndex);
					glVertexAttribIPointer(m_VertexBufferIndex, //layout(position = ${m_VertexBufferIndex})
						element.GetComponentCount(), //该组数据的数据单元个数
						ShaderDataTypeToOpenGLBaseType(element.Type),//该组数据的数据单元的类型
						layout.GetStride(),	 //该顶点数据的步长
						(const void*)element.Offset);//该组数据在顶点数据中的起始偏移量
					m_VertexBufferIndex++;
					break;
				}
				case ShaderDataType::Mat3:
				case ShaderDataType::Mat4:
				{
					uint8_t count = element.GetComponentCount();
					for(uint8_t i = 0; i < count; i++)
					{
						glEnableVertexAttribArray(m_VertexBufferIndex);
						glVertexAttribPointer(m_VertexBufferIndex, 
							count,
							ShaderDataTypeToOpenGLBaseType(element.Type),
							element.Normalized ? GL_TRUE : GL_FALSE,
							layout.GetStride(),
							(const void*)(element.Offset + sizeof(float) * count * i));
						glVertexAttribDivisor(m_VertexBufferIndex, 1);
						m_VertexBufferIndex++;
					}
					break;
				}
			default:
				XB_CORE_ASSERT(false, "Unknow ShaderDataType!")
				break;

			}
		}
		m_VertexBuffers.push_back(vertexBuffer);
	}

	void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
	{
		glBindVertexArray(m_RendererID);
		indexBuffer->Bind();
		m_IndexBuffer = indexBuffer;
	}

}

#include <XBai.h>
//入口点
#include "XBai/Core/EntryPoint.h"
#include "glm/gtc/matrix_transform.hpp"

#include "Platform/OpenGL/OpenGLShader.h"

#include <imgui/imgui.h>

#include <glm/gtc/type_ptr.hpp>

#include "EditorLayer.h"

namespace XBai
{
	class XBaiEditor :public Application
	{
	public:
		XBaiEditor(const ApplicationSpecification& specification)
			: Application(specification)
		{
			PushLayer(new EditorLayer());
		}

		~XBaiEditor() override
		{

		}
	};

	Application* CreateApplication(ApplicationCommandLineArgs args)
	{
		ApplicationSpecification spec;
		spec.Name = "XBai-Editor";
		spec.CommandLineArgs = args;
		return new XBaiEditor(spec);
	}
}

workspace "XBai"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "XBai/vendor/GLFW/include"
IncludeDir["Glad"] = "XBai/vendor/Glad/include"
IncludeDir["ImGui"] = "XBai/vendor/imgui"
IncludeDir["glm"] = "XBai/vendor/glm"

include "XBai/vendor/GLFW"
include "XBai/vendor/Glad"
include "XBai/vendor/imgui"

project "XBai"
	location "XBai"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "On"

	targetdir ("bin/"..outputdir.."/%{prj.name}")
	objdir ("bin-int/"..outputdir.."/%{prj.name}")
	buildoptions{"/utf-8"}

	pchheader "xbpch.h"
	pchsource "XBai/src/xbpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}"
	}

	links 
	{
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		
		systemversion "latest"
		defines
		{
			"XB_PLATFORM_WINDOWS",
			"XB_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

	filter "configurations:Debug"
		defines "XB_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "XB_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "XB_DIST"
		runtime "Release"
		optimize "On"

project "SandBox"
	location "SandBox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "On"

	targetdir ("bin/"..outputdir.."/%{prj.name}")
	objdir ("bin-int/"..outputdir.."/%{prj.name}")
	buildoptions{"/utf-8"}

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"XBai/vendor/spdlog/include",
		"XBai/src",
		"XBai/vendor",
		"%{IncludeDir.glm}"
	}

	links
	{
		"XBai"
	}

	filter "system:windows"
		
		systemversion "latest"
		defines
		{
			"XB_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "XB_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "XB_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "XB_DIST"
		runtime "Release"
		optimize "On"
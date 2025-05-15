project "XBai-Editor"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	targetdir ("%{wks.location}/bin/"..outputdir.."/%{prj.name}")
	objdir ("%{wks.location}/bin-int/"..outputdir.."/%{prj.name}")
	buildoptions{"/utf-8"}

	files
	{
		"src/**.h",
		"src/**.cpp"
	}

	includedirs
	{
		"%{wks.location}/XBai/vendor/spdlog/include",
		"%{wks.location}/XBai/src",
		"%{wks.location}/XBai/vendor",
		"%{IncludeDir.entt}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.ImGuizmo}"
	}

	links
	{
		"XBai"
	}

	filter "system:windows"
		systemversion "latest"


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
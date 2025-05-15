include "./vendor/premake/premake_customization/solution_items.lua"
include "Dependencies.lua"

workspace "XBai"
	architecture "x86_64"
	startproject "XBai-Editor"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

	solution_items
	{
		".editorconfig"
	}

	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencies"
	include "vendor/premake"
	include "XBai/vendor/Box2D"
	include "XBai/vendor/GLFW"
	include "XBai/vendor/Glad"
	--include "XBai/vendor/msdf-atlas-gen"
	include "XBai/vendor/imgui"
	include "XBai/vendor/yaml-cpp"
group ""

group "Core"
	include "XBai"
	--include "XBai-ScriptCore"
group ""

group "Tools"
	include "XBai-Editor"
group ""

group "Misc"
	include "Sandbox"
group ""
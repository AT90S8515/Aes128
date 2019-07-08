workspace "Aes128"
	architecture "x86"
	startproject "Aes128"

	configurations
	{
		"Debug",
		"Release",
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Aes128"
	location "Source"
	kind "ConsoleApp"
	language "C++"
	staticruntime "on"

	linkgroups "on"

	targetdir ("Build/" .. outputdir .. "/%{prj.name}")
	objdir ("Build-obj/" .. outputdir .. "/%{prj.name}")

	files 
	{
		"Source/**.h",
		"Source/**.cpp",
	}

	includedirs
	{
		"Source"
	}

	filter "system:windows"
		systemversion "latest"

	filter "system:linux"
		toolset "clang"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"
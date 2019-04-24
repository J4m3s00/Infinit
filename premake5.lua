workspace "Infinit"
	architecture "x64"

	configurations { "Debug", "Release"}


outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Infinit"
	location "Infinit"
	kind "SharedLib"
	language "C++"
	staticruntime "off"

	pchheader "inpch.h"
	pchsource "Infinit/src/inpch.cpp"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files 
	{
		"%{prj.name}/src/**.h",	
		"%{prj.name}/src/**.cpp"	
	}

	includedirs
	{
		"%{prj.name}/src/"
	}

	links
	{
		"opengl32.lib"
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

		defines 
		{
			"IN_PLATFORM_WINDOWS",
			"IN_BUILD_DLL"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} \"../bin/" .. outputdir .. "/Sandbox/\"")
		}

	filter "configurations:Debug"
		defines "IN_DEBUG"
		runtime "Debug"
		symbols "on"

		
	filter "configurations:Release"
		defines "IN_RELEASE"
		runtime "Release"
		optimize "on"


project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files 
	{
		"%{prj.name}/src/**.h",	
		"%{prj.name}/src/**.cpp"	
	}

	includedirs
	{
		"Infinit/src/",
		"%{prj.name}/src/"
	}

	links
	{
		"Infinit"
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

		defines 
		{
			"IN_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "IN_DEBUG"
		runtime "Debug"
		symbols "on"

		
	filter "configurations:Release"
		defines "IN_RELEASE"
		runtime "Release"
		optimize "on"
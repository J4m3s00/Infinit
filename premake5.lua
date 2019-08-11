workspace "Infinit"
	architecture "x64"
	startproject "Sandbox"

	configurations { "Debug", "Release"}


outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "Infinit/vendor/GLFW/include"
IncludeDir["Glad"] = "Infinit/vendor/Glad/include"
IncludeDir["spdlog"] = "Infinit/vendor/spdlog/include"
IncludeDir["glm"] = "Infinit/vendor/glm"
IncludeDir["imgui"] = "Infinit/vendor/imgui"
IncludeDir["Assimp"] = "Infinit/vendor/Assimp/include"
IncludeDir["lua"] = "Infinit/vendor/lua"

include "Infinit/vendor/GLFW"
include "Infinit/vendor/Glad"
include "Infinit/vendor/imgui"
include "Infinit/vendor/lua"

project "Infinit"
	location "Infinit"
	kind "StaticLib"
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
		"%{prj.name}/src/",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.imgui}",
		"Infinit/vendor/stb/",
		"%{IncludeDir.Assimp}",
		"%{IncludeDir.lua}"
	}

	links
	{
		"GLFW",
		"Glad",
		"imgui",
		"lua",
		"Infinit/vendor/Assimp/lib/x64/assimp-vc140-mt.lib",
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

--		postbuildcommands
--		{
--			("{COPY} %{cfg.buildtarget.relpath} \"../bin/" .. outputdir .. "/Sandbox/\"")
--		}


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

--	targetdir ("bin/" .. outputdir .. "/Infinit")
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
		"%{prj.name}/src/",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.imgui}",
		"%{IncludeDir.glm}"
	}

	links
	{
		"Infinit",
		"imgui"
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
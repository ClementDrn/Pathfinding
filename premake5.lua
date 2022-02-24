--- Solution -------------------------------
workspace "Pathfinding"
   
	configurations { 
	   "Debug", 
	   "Release"
	}

	platforms {
		"x86",
		"x64"
	}

	startproject "App"

	filter "platforms:x86"
		architecture "x86"

	filter "platforms:x64"
		architecture "x86_64"


--- Variables ------------------------------
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

includedir = {}
includedir["SFML"] = "App/vendor/SFML/include"
includedir["SEL"] = "Pathfinding/vendor/SEL/include"
includedir["Pathfinding"] = "Pathfinding/include"


--- Dependencies ---------------------------
group "Dependencies"
	include "App/vendor/SFML"
group ""


--- Pathfinding --------------------------------
project "Pathfinding"
	
	kind "StaticLib"
	location "Pathfinding"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"

	targetdir("bin/" .. outputdir .. "/%{prj.name}")
	objdir("obj/" .. outputdir .. "/%{prj.name}")

	files {
		"%{prj.name}/src/**.hpp",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/include/**.hpp"
	}

	includedirs {
		"%{includedir.SEL}",
		"Pathfinding/include/",
		"Pathfinding/src/"
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"


--- App --------------------------------
project "App"

	location "App"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"

	targetdir("bin/" .. outputdir .. "/%{prj.name}")
	objdir("obj/" .. outputdir .. "/%{prj.name}")

	files {
		"%{prj.name}/src/**.hpp",
		"%{prj.name}/src/**.cpp"
	}

	includedirs {
		"%{includedir.Pathfinding}",
		"%{includedir.SFML}",
		"%{includedir.SEL}",
	}

	defines {
		"SFML_STATIC"
	}

	filter "system:windows"
		entrypoint "mainCRTStartup"
		systemversion "latest"
		libdirs {
			"%{prj.name}/vendor/SFML/extlibs/libs-msvc-universal/%{cfg.platform}/"
		}
	
	filter {}
	links {
		"opengl32.lib",
		"freetype.lib",
		"winmm.lib",
		"gdi32.lib",
		"ws2_32.lib",
		"SFML",
		"Pathfinding"
	}

	filter "configurations:Debug"
		kind "ConsoleApp"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		kind "WindowedApp"
		runtime "Release"
		optimize "on"

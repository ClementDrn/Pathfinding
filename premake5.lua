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

	startproject "Pathfinding"

	filter "platforms:x86"
		architecture "x86"

	filter "platforms:x64"
		architecture "x86_64"


--- Variables ------------------------------
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

includedir = {}
includedir["SFML"] = "Pathfinding/vendor/SFML/include"


--- Dependencies ---------------------------
group "Dependencies"
	include "Pathfinding/vendor/SFML"
group ""


--- Pathfinding --------------------------------
project "Pathfinding"

	location "Pathfinding"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"

	targetdir("bin/" .. outputdir .. "/%{prj.name}")
	objdir("bin-int/" .. outputdir .. "/%{prj.name}")

	files {
		"%{prj.name}/src/**.hpp",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.c"
	}

	includedirs {
		"%{includedir.SFML}"
	}

	defines {
		"SFML_STATIC"
	}

	filter "system:windows"
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
		"SFML"
	}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"

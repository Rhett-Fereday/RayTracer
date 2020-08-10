workspace "RayTracer"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release"
	}
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["glm"] = "RayTracer/lib/glm"
IncludeDir["CImg"] = "RayTracer/lib/CImg"

project "RayTracer"
	location "RayTracer"
	kind "SharedLib"
	language "C++"
	cppdialect "C++17"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	postbuildcommands
	{
		("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/TestProgram")
	}

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{IncludeDir.glm}",
		"%{IncludeDir.CImg}"
	}

project "TestProgram"
	location "TestProgram"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"RayTracer/src",
		"%{IncludeDir.glm}",
		"%{IncludeDir.CImg}"
	}

	links
	{
		"RayTracer.dll"
	}
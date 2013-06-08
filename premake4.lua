-------------------------------------------------------------------------------
----	Plasma Premake File
----		-> Main solution
----
----	Author: Chirag Sangani
----			csangani@stanford.edu
----
----	Date:	5/23/2013
-------------------------------------------------------------------------------

--	Project Defines.

SolutionName		= "Plasma"

Platforms			= { "native" }

Debug				= "Debug"
Release				= "Release"

BuildPath			= "build/"
LibPath				= "lib/"

IncludePath			= "include/"

Defines				= { "FREEGLUT_STATIC", "GLEW_STATIC", "_USE_MATH_DEFINES",
						"_SCL_SECURE_NO_WARNINGS", "_CRT_SECURE_NO_WARNINGS" }

DebugFlags			= { "Symbols", "StaticRuntime", "EnableSSE2", "FloatFast" }
ReleaseFlags		= { "OptimizeSpeed", "StaticRuntime", "EnableSSE2",
						"FloatFast" }

--	Convenience defines.

IncludeDirs			= { IncludePath, "Bullet/" }

Configurations		= { Debug, Release }
	
DebugBuildPath		= path.join ((BuildPath), (Debug))
ReleaseBuildPath	= path.join ((BuildPath), (Release))
	
DebugLibPath		= path.join ((LibPath), (Debug))
ReleaseLibPath		= path.join ((LibPath), (Release))

--	Static library definitions.

CPP					= "C++"
C					= "C"

function DefineStaticLib (ProjectName, ProjectDir, TargetDir, Language)
	project (ProjectName)
		kind "StaticLib"
		language (Language)
		location (ProjectDir)
		targetname (ProjectName)
		includedirs (IncludeDirs)
		if Language == C then
			files { path.join(path.join (IncludePath, ProjectDir), "**.h"),
				path.join(ProjectDir, "**.h"),
				path.join(ProjectDir, "**.c"),
				path.join(ProjectDir, "**.rc") }
		elseif Language == CPP then
			files { path.join(path.join (IncludePath, ProjectDir), "**.h"),
				path.join(path.join (IncludePath, ProjectDir), "**.hh"),
				path.join(path.join (IncludePath, ProjectDir), "**.hpp"),
				path.join(ProjectDir, "**.h"),
				path.join(ProjectDir, "**.hh"),
				path.join(ProjectDir, "**.hpp"),
				path.join(ProjectDir, "**.cc"),
				path.join(ProjectDir, "**.cpp"),
				path.join(ProjectDir, "**.rc") }
		else
			error ("Invalid language specified.", 2)
		end

		configuration (Debug)
			defines (Defines)
			flags (DebugFlags)
			targetdir (path.join (DebugLibPath, TargetDir))
			objdir (path.join (DebugBuildPath, TargetDir))

		configuration (Release)
			defines (Defines)
			flags (ReleaseFlags)
			targetdir (path.join (ReleaseLibPath, TargetDir))
			objdir (path.join (ReleaseBuildPath, TargetDir))
end

-- Define the solution.

solution (SolutionName)
	configurations		(Configurations)
	platforms			(Platforms)
	
	-- Spline
	Spline = "Spline"
	DefineStaticLib (Spline, "Spline/", "Spline/", CPP)

	-- PolyMesh
	PolyMesh = "PolyMesh"
	DefineStaticLib (PolyMesh, "PolyMesh/", "PolyMesh/", CPP)

	-- OpenMesh
	OpenMeshCore = "OpenMeshCore"
	DefineStaticLib (OpenMeshCore, "OpenMesh/Core/", "OpenMesh/Core/", CPP)

	-- GLEW
	GLEW = "GLEW"
	DefineStaticLib (GLEW, "GLEW/", "GLEW/", C)

	-- FreeGLUT
	FreeGLUT = "FreeGLUT"
	DefineStaticLib (FreeGLUT, "FreeGLUT/", "FreeGLUT/", C)

	-- Bullet
	Bullet = "Bullet"
	DefineStaticLib (Bullet, "Bullet/", "Bullet/", CPP)

	-- Shad
	Shad = "Shad"
	project (Shad)
		kind "ConsoleApp"
		language "C++"
		location ("Shad/")
		targetname (Shad)
		includedirs (IncludeDirs)
		files { path.join (IncludePath, "Shad/**.h"),
				path.join (IncludePath, "Shad/**.hh"),
				path.join (IncludePath, "Shad/**.hpp"),
				"Shad/**.cpp", "Shad/**.rc" }
		links { Spline, OpenMeshCore, GLEW, FreeGLUT, PolyMesh, Bullet, 
				path.join (path.join ("../", LibPath), "FMODex/FMODex") }

		configuration "Debug"
			defines (Defines)
			flags (DebugFlags)
			targetdir (DebugBuildPath)
			libdirs { DebugLibPath }
			objdir (path.join (DebugBuildPath, "Shad/"))
			postbuildcommands { "xcopy Assets ..\\Build\\Debug\\Assets /E /I /Y",
								"xcopy dll\\* ..\\Build\\Debug /E /I /Y" }

		configuration "Release"
			defines (Defines)
			flags (ReleaseFlags)
			targetdir (ReleaseBuildPath)
			libdirs { ReleaseLibPath }
			objdir (path.join (ReleaseBuildPath, "Shad/"))
			postbuildcommands { "xcopy Assets ..\\Build\\Release\\Assets /E /I /Y",
								"xcopy dll\\* ..\\Build\\Release /E /I /Y" }
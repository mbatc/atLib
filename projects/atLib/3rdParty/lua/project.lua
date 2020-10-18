project "lua"
configurations { "Debug", "Release" }

kind "StaticLib"
architecture "x64"
language "C++"
characterset ("MBCS")

-- Set Directories

bin_path = atLibBuildPath

symbolspath '$(OutDir)$(TargetName).pdb'
targetdir (bin_path)
debugdir (bin_path)
objdir "../../builds/output/%{cfg.platform}_%{cfg.buildcfg}"

-- Project Flags

flags { "FatalWarnings" }
flags { "MultiProcessorCompile" }

-- Linker options

linkoptions { "/ignore:4006" }
linkoptions { "/ignore:4221" }
linkoptions { "/ignore:4075" }

-- Shared Defines

  -- defines { "_CRT_SECURE_NO_WARNINGS", "ATLIB_DIRECTX" }

-- Project Files
  includedirs { "include", "include/**" }
  files { "source/**.c" }
  files { "include/**.h" }
  files { "include/**.hpp" }

-- Debug Configuration Settings

  filter { "configurations:Debug" }
    defines { "DEBUG"}
    symbols "On"
    editandcontinue "On"

-- Release Configuration Settings

  filter { "configurations:Release" }
    flags { "LinkTimeOptimization" }
    defines { "NDEBUG" }
    optimize "On"
    editandcontinue "Off"
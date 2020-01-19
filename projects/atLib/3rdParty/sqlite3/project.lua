project "sqlite3"
configurations { "Debug", "Release" }

kind "StaticLib"
architecture "x64"
language "C++"
characterset ("MBCS")

-- Set Directories

symbolspath '$(OutDir)$(TargetName).pdb'
targetdir "../../../../builds/bin/"
debugdir "../../../../builds/bin/"
objdir "../../../../builds/output/%{cfg.platform}_%{cfg.buildcfg}"

-- Project Flags

flags { "FatalWarnings" }
flags { "MultiProcessorCompile" }

-- Linker options

linkoptions { "/ignore:4006" }
linkoptions { "/ignore:4221" }

-- Shared Defines

  -- defines { "_CRT_SECURE_NO_WARNINGS", "ATLIB_DIRECTX" }

-- Project Files

  files { "source/sqlite3.c", "include/sqlite3.h", "include/sqlite3ext.h", }

-- Debug Configuration Settings

  filter { "configurations:Debug" }
    defines { "DEBUG"}
    symbols "On"

-- Release Configuration Settings

  filter { "configurations:Release" }
    flags { "LinkTimeOptimization" }
    defines { "NDEBUG" }
    optimize "On"
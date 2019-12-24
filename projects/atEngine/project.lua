CTML_LIB = 1
CTML_BIN_DIR = "../../../../builds/bin"
CTML_OUT_DIR = "../../../../builds/output/CTML"

dofile "3rdParty/CTML/project.lua"
  location "3rdParty/CTML"

project "atEngine"

dependson { "atLib" }
dependson { "CTML" }

configurations { "Debug", "Release" }

kind "ConsoleApp"
architecture "x64"
language "C++"
characterset ("MBCS")

-- Set Directories

symbolspath '$(OutDir)$(TargetName).pdb'
targetdir "../../builds/bin/"
debugdir "../../builds/bin/"
objdir "../../builds/output/atEngine/%{cfg.platform}_%{cfg.buildcfg}"

-- Project Flags

flags { "FatalWarnings" }
flags { "MultiProcessorCompile" }

-- Shared Defines

  defines { "_CRT_SECURE_NO_WARNINGS" }

-- Includes

  -- atLib Includes
  includedirs { "include", "include/**" } 
  includedirs { "source", "source/**" } 
  includedirs { "../atLib/include", "../atLib/include/**" }

-- Project Files

  files { "project.lua" } -- include project file so it can be edited in IDE

  files { "source/**.cpp", "source/**.h", "source/**.inl" , "**.natvis" }

  links { "atLib" }
  links { "CTML" }

-- Debug Configuration Settings

  filter { "configurations:Debug" }
    defines { "DEBUG"}
    symbols "On"
    libdirs {"../atLib/3rdParty/python/libs/Debug/"}

-- Release Configuration Settings

  filter { "configurations:Release" }
    flags { "LinkTimeOptimization" }
    defines { "NDEBUG" }
    optimize "On"
    libdirs {"../3rdParty/python/libs/Release/"}
  
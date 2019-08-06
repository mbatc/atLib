project "atTest"
dependson { "atLib" }
configurations { "Debug", "Release" }

kind "ConsoleApp"
architecture "x64"
language "C++"
characterset ("MBCS")

-- Set Directories

symbolspath '$(OutDir)$(TargetName).pdb'
targetdir "../../builds/bin/"
debugdir "../../builds/bin/"
objdir "../../builds/output/%{cfg.platform}_%{cfg.buildcfg}"

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

  -- Third Party
  includedirs { "3rdParty/**" } 

-- Third Party Files

  files { "3rdParty/**.cpp", "3rdParty/**.h", "3rdParty/**.inl" }

-- Project Files

  files { "source/**.cpp", "source/**.h", "source/**.inl" , "**.natvis" }

  links { "atLib" }

-- Debug Configuration Settings

  filter { "configurations:Debug" }
    defines { "DEBUG"}
    symbols "On"

-- Release Configuration Settings

  filter { "configurations:Release" }
    flags { "LinkTimeOptimization" }
    defines { "NDEBUG" }
    optimize "On"
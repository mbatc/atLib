project "atLib"
configurations { "Debug", "Release" }

kind "StaticLib"
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

-- Build Options

buildoptions { "/bigobj" }

-- Shared Defines

  defines { "_CRT_SECURE_NO_WARNINGS", "ATLIB_DIRECTX" }

-- Includes

  -- atLib Includes
  includedirs { "include", "include/**" } 

  -- Third Party
  includedirs { "3rdParty/**" } 


-- Third Party Files

  files { "3rdParty/**.cpp", "3rdParty/**.h", "3rdParty/**.inl" }
  files { "3rdParty/sol/sol.hpp" }

-- Project Files

  files { "source/**.cpp", "include/**.h", "include/**.inl" , "**.natvis" }

  links { "LuaLib" }

-- Debug Configuration Settings

  filter { "configurations:Debug" }
    defines { "DEBUG"}
    symbols "On"
    libdirs {"3rdParty/lua/Debug/"}

-- Release Configuration Settings

  filter { "configurations:Release" }
    flags { "LinkTimeOptimization" }
    defines { "NDEBUG" }
    optimize "On"
    libdirs {"3rdParty/lua/Release/"}
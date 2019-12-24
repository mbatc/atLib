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
objdir "../../builds/output/atLib/%{cfg.platform}_%{cfg.buildcfg}"

-- Project Flags

flags { "FatalWarnings" }
flags { "MultiProcessorCompile" }

-- Build Options

-- buildoptions { "/bigobj" }

-- Linker options

linkoptions { "/ignore:4006" }
linkoptions { "/ignore:4221" }

-- Shared Defines

  defines { "_CRT_SECURE_NO_WARNINGS", "ATLIB_DIRECTX" }

-- Includes

  -- atLib Includes
  includedirs { "include", "include/**" } 

  -- Third Party
  includedirs { "3rdParty" } 
  includedirs { "3rdParty/stb" }
  includedirs { "3rdParty/lua/include" }
  includedirs { "3rdParty/glew/include" }
  includedirs { "3rdParty/imgui" }
  includedirs { "3rdParty/python/include" }


-- Third Party Files

  files { "3rdParty/sol/sol.hpp" }
  files { "3rdParty/imgui/**.cpp", "3rdParty/imgui/**.h" }

-- Project Files

  files { "project.lua" } -- include project file so it can be edited in IDE
  files { "source/**.cpp", "include/**.h", "include/**.inl" , "**.natvis" }

  files { "source/**.py" } -- scene scripting interface source files

  links { "LuaLib" }
  links { "opengl32", "glew32" }
  libdirs { "3rdParty/glew/lib/Release/x64/" }

-- Debug Configuration Settings

  filter { "configurations:Debug" }
    defines { "DEBUG"}
    symbols "On"
    libdirs {"3rdParty/lua/Debug/"}
    libdirs {"3rdParty/python/libs/Debug/"}

-- Release Configuration Settings

  filter { "configurations:Release" }
    flags { "LinkTimeOptimization" }
    defines { "NDEBUG" }
    optimize "On"
    libdirs {"3rdParty/lua/Release/"}
    libdirs {"3rdParty/python/libs/Release/"}
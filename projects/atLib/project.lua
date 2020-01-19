dofile "projects/atLib/3rdParty/sqlite3/project.lua"
  location "3rdParty/sqlite3/"

project "atLib"
configurations { "Debug", "Release" }

dependson { "sqlite3" }

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


-- Third Party Files

  files { "3rdParty/sol/sol.hpp" }
  files { "3rdParty/imgui/**.cpp", "3rdParty/imgui/**.h" }

-- Project Files

  files { "source/**.cpp", "include/**.h", "include/**.inl" , "**.natvis" }

  links { "LuaLib" }
  links { "sqlite3" }
  links { "opengl32", "glew32" }
  libdirs { "3rdParty/glew/lib/Release/x64/" }

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

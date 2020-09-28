
if (not atLibBuildPath) then
  atLibBuildPath = "..\\..\\builds\\bin"
end

win32Build = os.target() == "windows"
linuxBuild = os.target() == "linux"

dofile "3rdParty/sqlite3/project.lua"
location "../3rdParty/sqlite3/"

dofile "3rdParty/lua/project.lua"
location "../3rdParty/lua/"

project "atLib"
configurations { "Debug", "Release" }

if (not win32Build and not linuxBuild) then
  print("This operating system is not supported.")
  exit()
end

-- Setup WIN32 Project
if (win32Build == true) then
  defines { "atPLATFORM_WIN32" }
  print("Creating project for Windows...")
end

-- Setup Linux Project
if (linuxBuild == true) then
  defines { "atPLATFORM_LINUX" }
  includedirs { "/usr/include" }
  libdirs {"/usr/lib"}
  print("Creating project for Linux...")
end

dependson { "lua" }
dependson { "sqlite3" }

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

-- Build Options

-- buildoptions { "/bigobj" }

-- Linker options

linkoptions { "/ignore:4006" }
linkoptions { "/ignore:4221" }
linkoptions { "/ignore:4075" }

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
  includedirs { "3rdParty/fbxsdk/include" }
  includedirs { "3rdParty/nanoSVG/src" }


-- Third Party Files

  files { "3rdParty/sol/sol.hpp" }
  files { "3rdParty/imgui/**.cpp", "3rdParty/imgui/**.h" }

-- Project Files

  files { "source/**.cpp", "include/**.h", "include/**.inl" , "**.natvis" }

  links { "lua" }
  links { "sqlite3" }
  links { "libfbxsdk" }
  links { "opengl32", "glew32" }
  
  libdirs { "3rdParty/glew/lib/Release/x64/" }

-- Debug Configuration Settings

  filter { "configurations:Debug" }
    defines { "DEBUG"}
    symbols "On"
	  editandcontinue "On"
    libdirs { "3rdParty/fbxsdk/lib/x64/debug" }
    
    -- Copy PDB's and DLL's
    postbuildcommands { "copy \"3rdParty\\lua\\Debug\\LuaLib.pdb\" \"" .. bin_path .. "\\LuaLib.pdb\" /y" }
    postbuildcommands { "copy \"3rdParty\\fbxsdk\\lib\\x64\\debug\\libfbxsdk.dll\" \"" .. bin_path .. "\\libfbxsdk.dll\" /y" }
    postbuildcommands { "copy \"3rdParty\\glew\\bin\\Release\\x64\\glew32.dll\" \"" .. bin_path .. "\\glew32.dll\" /y" }

-- Release Configuration Settings

  filter { "configurations:Release" }
    flags { "LinkTimeOptimization" }
    defines { "NDEBUG" }
    optimize "On"
	  editandcontinue "Off"
    libdirs { "3rdParty/fbxsdk/lib/x64/release" }

    -- Copy PDB's and DLL's
    postbuildcommands { "copy \"3rdParty\\lua\\Release\\LuaLib.pdb\" \"" .. bin_path .. "\\LuaLib.pdb\" /y" }
    postbuildcommands { "copy \"3rdParty\\fbxsdk\\lib\\x64\\release\\libfbxsdk.dll\" \"" .. bin_path .. "\\libfbxsdk.dll\" /y" }
    postbuildcommands { "copy \"3rdParty\\glew\\bin\\Release\\x64\\glew32.dll\" \"" .. bin_path .. "\\glew32.dll\" /y" }

project "atLib"
configurations { "Debug", "Release" }

kind "StaticLib"
architecture "x64"
language "C++"
characterset ("MBCS")

-- Set Directories --

symbolspath '$(OutDir)$(TargetName).pdb'
targetdir "../../builds/bin/"
debugdir "../../builds/bin/"
objdir "../../builds/output/%{cfg.platform}_%{cfg.buildcfg}"

-- Project Flags --

flags { "FatalWarnings" }
flags { "MultiProcessorCompile" }

-- Build Options --

buildoptions { "/bigobj" }

-- Shared Defines

  defines { "_CRT_SECURE_NO_WARNINGS", "ATLIB_DIRECTX", "USE_STANDALONE_ASIO", "ASIO_STANDALONE" }

-- Includes

  -- atLib Includes --
  includedirs { "include", "include/**" } 
  
  -- Networking
  includedirs { "3rdParty/asio/include"}
  includedirs { "3rdParty/openssl/include" }
  includedirs { "3rdParty/SimpleWebServer" }

  -- Lua Libs 
  includedirs { "3rdParty/lua/include" } 
  includedirs { "3rdParty/sol" } 

  -- Other
  includedirs { "3rdParty/stb" } 

-- Third Party Files -- 

  files { "3rdParty/imgui/*.cpp",  "3rdParty/imgui/*.h" } -- ImGui Files

-- Project Files --

  files { "source/**.cpp", "include/**.h", "include/**.inl" , "**.natvis" }

  links { "LuaLib" }
  links { "ssleay32" }

-- Debug Configuration Settings --

  filter { "configurations:Debug" }
    defines { "DEBUG"}
    symbols "On"
    libdirs {"3rdParty/lua/Debug/"}
    libdirs {"3rdParty/openssl/lib/debug"}

-- Release Configuration Settings --###

  filter { "configurations:Release" }
    flags { "LinkTimeOptimization" }
    defines { "NDEBUG" }
    optimize "On"
    libdirs {"3rdParty/lua/Release/"}
    libdirs {"3rdParty/openssl/lib/release"}
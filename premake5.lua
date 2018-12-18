workspace "aberrantToothbrushLibrary"
  configurations {"Debug", "Release" }
  startproject "atEngine"

dofile "projects/atLib/project.lua"
  location "projects/atLib/"
  
dofile "projects/atTest/project.lua"
  location "projects/atTest/"

dofile "projects/atEngine/project.lua"
  location "projects/atEngine/"
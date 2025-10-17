workspace "KeyActions"
   architecture "x64"
   configurations { "Debug", "Release", "Dist" }
   startproject "KeyActions"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "lumina/lumina-external.lua"

group "App"
   include "key-actions"
group ""
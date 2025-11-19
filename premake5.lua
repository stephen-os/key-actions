workspace "KeyActions"
   architecture "x64"
   configurations { "Debug", "Release", "Dist" }
   startproject "KeyActionsApp"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "lumina/lumina-external.lua"

group "App"
   include "key-actions"
group ""

group "Tests"
   include "tests/nodes"
   include "tests/node-graph"
group ""
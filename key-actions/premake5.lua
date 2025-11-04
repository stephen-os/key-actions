project "KeyActions"
   kind "ConsoleApp"
   language "C++"
   cppdialect "C++20"
   targetdir "bin/%{cfg.buildcfg}"
   staticruntime "off"

   flags { "MultiProcessorCompile" }

   files { "src/**.h", "src/**.cpp" }

   includedirs
   {
      "%{wks.location}/key-actions/src",

      "%{wks.location}/lumina/lumina/src",

      "%{wks.location}/lumina/dependencies/imgui",
      "%{wks.location}/lumina/dependencies/glew/include",
      "%{wks.location}/lumina/dependencies/glfw/include",
      "%{wks.location}/lumina/dependencies/glm",
      "%{wks.location}/lumina/dependencies/glad/include",
      "%{wks.location}/lumina/dependencies/tinygltf",
      "%{wks.location}/lumina/dependencies/imguifd",
      "%{wks.location}/lumina/dependencies/spdlog/include",
      "%{wks.location}/lumina/dependencies/imgui-node-editor",
      "%{wks.location}/lumina/dependencies/imgui-node-editor/external/DXSDK/include"
   }

   links
   {
      "Lumina",
   }

   buildoptions { "/utf-8" }

   targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
   objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

   filter "system:windows"
      systemversion "latest"
      defines { "LUMINA_PLATFORM_WINDOWS" }

   filter "configurations:Debug"
      defines { "LUMINA_DEBUG" }
      runtime "Debug"
      symbols "On"
      optimize "Off"

   filter "configurations:Release"
      defines { "LUMINA_RELEASE" }
      runtime "Release"
      optimize "Speed"
      symbols "On"

   filter "configurations:Dist"
      kind "WindowedApp"
      defines { "LUMINA_DIST" }
      runtime "Release"
      optimize "Speed"
      symbols "Off"
workspace "thebanana"
	architecture "x64"
	configurations {
        "Debug",
        "Release"
    }
    flags {
        "MultiProcessorCompile"
    }
    startproject "bananatree"
    filter "system:windows"
        defines {
            "BANANA_WINDOWS"
        }
    filter "system:macosx"
        defines {
            "BANANA_MACOSX"
        }
    filter "system:linux"
        defines {
            "BANANA_LINUX"
        }
    filter "configurations:Debug"
        defines {
            "BANANA_DEBUG"
        }
        symbols "on"
    filter "configurations:Release"
        defines {
            "BANANA_RELEASE"
        }
        optimize "on"
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
newoption {
    trigger = "renderer",
    value = "API",
    description = "The 3D rendering API to use",
    allowed = {
        { "opengl", "OpenGL" },
        { "none", "None" }
    }
}
if not _OPTIONS["renderer"] then
    _OPTIONS["renderer"] = "opengl"
end
group "misc"
project "configfiles"
    kind "None"
    files {
        "premake5.lua",
        "README.md",
        ".gitignore"
    }
project "docs"
    location "docs"
    kind "None"
    files {
        "%{prj.name}/**.md",
    }
group ""
group "dependencies"
project "imgui"
    location "imgui"
    kind "SharedLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "off"
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
    files {
        "%{prj.name}/imgui/*.h",
        "%{prj.name}/imgui/*.cpp",
        "%{prj.name}/imgui/backends/imgui_impl_opengl3.*",
        "%{prj.name}/imgui/backends/imgui_impl_win32.*",
        "%{prj.name}/imgui/misc/cpp/imgui_stdlib.*",
        "imguizmo/ImGuizmo/ImGuizmo.*",
    }
    includedirs {
        "%{prj.name}/imgui"
    }
    sysincludedirs {
        "vendor/glew/include",
        "vendor/freetype2/include"
    }
    syslibdirs {
        "vendor/freetype2/lib/%{cfg.buildcfg}",
        "vendor/glew/lib/%{cfg.buildcfg}"
    }
    defines {
        "_IMGUI_BUILD_DLL",
        "_IMGUI_BUILD",
    }
    filter "system:windows"
        links {
            "opengl32.lib"
        }
    filter { "system:windows", "configurations:Debug" }
        links {
            "freetyped.lib",
            "glew32d.lib"
        }
    filter { "system:windows", "configurations:Release" }
        links {
            "freetype.lib",
            "glew32.lib"
        }
project "yaml"
    location "yaml"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "off"
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
    files {
        "%{prj.name}/yaml-cpp/src/**.h",
        "%{prj.name}/yaml-cpp/src/**.cpp",
        "%{prj.name}/yaml-cpp/include/**.h"
    }
    includedirs {
        "%{prj.name}/yaml-cpp/include"
    }
group ""
group "core"
project "thebanana"
    location "thebanana"
    kind "SharedLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "off"
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
    pchheader "pch.h"
    pchsource "%{prj.name}/src/pch.cpp"
    files {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/include/**.h",
        "%{prj.name}/os-src/*.h",
        "%{prj.name}/os-src/%{cfg.system}/**.h",
        "%{prj.name}/os-src/%{cfg.system}/**.cpp",
    }
    includedirs {
        "%{prj.name}/src",
        "%{prj.name}/include",
        "%{prj.name}/os-src"
    }
    sysincludedirs {
        "vendor/assimp/include",
        "vendor/mono/include",
        "yaml/yaml-cpp/include",
        "vendor/steam-sdk/include",
        "third-party/minimp3",
        "third-party/stb",
        "third-party/json/single-include",
        "imgui/imgui",
        "glm",
        "vendor/glew/include",
    }
    defines {
        "_IMGUI_BUILD_DLL",
        "BANANA_BUILD",
    }
    links {
        "imgui",
        "yaml"
    }
    filter "options:renderer=opengl"
        defines {
            "RENDERER_OPENGL"
        }
    filter "system:windows"
        syslibdirs {
            "vendor/assimp/lib/%{cfg.buildcfg}/",
            "vendor/mono/lib/%{cfg.buildcfg}",
            "vendor/glew/lib/%{cfg.buildcfg}",
            "vendor/steam-sdk/lib"
        }
        links {
            "opengl32.lib",
            "dinput8.lib",
            "steam_api64.lib",
            "mono-2.0-sgen.lib",
        }
    filter { "system:windows", "configurations:Debug" }
        links {
            "assimp-vc142-mtd.lib",
            "glew32d.lib"
        }
    filter { "system:windows", "configurations:Release" }
        links {
            "assimp-vc142-mt.lib",
            "glew32.lib"
        }
    filter { "system:windows", "action:vs*" }
        sysincludedirs {
            "$(DXSDK_DIR)include"
        }
group ""
group "tools"
project "bananatree"
    location "bananatree"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "off"
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
    files {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
    }
    sysincludedirs {
        "thebanana/include",
        "glm",
        "vendor/glew/include",
        "imgui/imgui",
        "yaml/yaml-cpp/include",
        "imguizmo/ImGuizmo"
    }
    links {
        "thebanana",
        "yaml",
        "imgui",
    }
    defines {
        "_IMGUI_BUILD_DLL",
    }
    filter "system:windows"
        links {
            "opengl32.lib"
        }
        postbuildcommands {
            '{COPY} "%{cfg.targetdir}/../thebanana/thebanana.dll" "%{cfg.targetdir}"',
            '{COPY} "%{cfg.targetdir}/../imgui/imgui.dll" "%{cfg.targetdir}"',
            '{COPY} "../vendor/steam-sdk/bin/steam_api64.dll" "%{cfg.targetdir}"',
        }
    filter { "system:windows", "configurations:Debug" }
        postbuildcommands {
            '{COPY} "../vendor/assimp/bin/Debug/assimp-vc142-mtd.dll" "%{cfg.targetdir}"',
            '{COPY} "../vendor/glew/bin/Debug/glew32d.dll" "%{cfg.targetdir}"',
            '{COPY} "../vendor/glew/bin/Release/glew32.dll" "%{cfg.targetdir}"',
        }
    filter { "system:windows", "configurations:Release" }
        postbuildcommands {
            '{COPY} "../vendor/assimp/bin/Release/assimp-vc142-mt.dll" "%{cfg.targetdir}"',
            '{COPY} "../vendor/glew/bin/Release/glew32.dll" "%{cfg.targetdir}"',
        }
    filter { "system:windows", "action:vs*" }
        linkoptions {
            "/subsystem:windows"
        }
project "sandbox"
    location "sandbox"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "off"
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
    files {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
    }
    sysincludedirs {
        "thebanana/include",
        "glm",
        "yaml/yaml-cpp/include",
    }
    links {
        "thebanana",
        "yaml",
    }
    filter "system:windows"
        syslibdirs {
            "vendor/glew/lib/%{cfg.buildcfg}",
        }
        links {
            "opengl32.lib",
        }
        postbuildcommands {
            '{COPY} "%{cfg.targetdir}/../thebanana/thebanana.dll" "%{cfg.targetdir}"',
            '{COPY} "%{cfg.targetdir}/../imgui/imgui.dll" "%{cfg.targetdir}"',
            '{COPY} "../vendor/steam-sdk/bin/steam_api64.dll" "%{cfg.targetdir}"',
        }
    filter { "system:windows", "configurations:Debug" }
        postbuildcommands {
            '{COPY} "../vendor/assimp/bin/Debug/assimp-vc142-mtd.dll" "%{cfg.targetdir}"',
            '{COPY} "../vendor/glew/bin/Debug/glew32d.dll" "%{cfg.targetdir}"',
            '{COPY} "../vendor/glew/bin/Release/glew32.dll" "%{cfg.targetdir}"',
        }
        links {
            "glew32d.lib"
        }
    filter { "system:windows", "configurations:Release" }
        postbuildcommands {
            '{COPY} "../vendor/assimp/bin/Release/assimp-vc142-mt.dll" "%{cfg.targetdir}"',
            '{COPY} "../vendor/glew/bin/Release/glew32.dll" "%{cfg.targetdir}"',
        }
        links {
            "glew32.lib"
        }
    filter { "system:windows", "action:vs*" }
        linkoptions {
            "/subsystem:windows"
        }
group ""
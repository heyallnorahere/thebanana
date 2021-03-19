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
            "BANANA_WINDOWS",
            "_IMGUI_BUILD_DLL"
        }
    filter "system:macosx"
        defines {
            "BANANA_MACOSX",
            "_IMGUI_BUILD_DYLIB"
        }
    filter "system:linux"
        defines {
            "BANANA_LINUX",
            "_IMGUI_BUILD_SO"
        }
        buildoptions {
            "-fpermissive",
            "-fPIC",
        }
        sysincludedirs {
            "/usr/include"
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
    kind "Utility"
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
    files {
        "premake5.lua",
        "README.md",
        ".gitignore"
    }
project "docs"
    location "docs"
    kind "Utility"
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
    files {
        "%{prj.name}/**.md",
    }
group ""
group "dependencies"
include "vendor/glad"
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
        "%{prj.name}/imgui/misc/cpp/imgui_stdlib.*",
        "imguizmo/ImGuizmo/ImGuizmo.*",
    }
    includedirs {
        "%{prj.name}/imgui"
    }
    sysincludedirs {
        "vendor/freetype2/include"
    }
    syslibdirs {
        "vendor/freetype2/lib/%{cfg.buildcfg}",
    }
    defines {
        "_IMGUI_BUILD",
    }
    filter "system:not macosx"
        sysincludedirs {
            "vendor/glew/include",
        }
        syslibdirs {
            "vendor/glew/lib/%{cfg.buildcfg}"
        }
    filter "system:linux"
        syslibdirs {
            "/usr/lib/x86_64-linux-gnu"
        }
        links {
            "X11",
            "GLEW",
            "GLU",
            "GL",
            "xcb",
            "X11-xcb",
            "xcb-cursor",
            "xcb-keysyms",
            "xcb-randr",
            "xcb-xkb",
            "xcb-xfixes"
        }
        files {
            "%{prj.name}/extensions/backends/imgui_impl_x11.*"
        }
    filter "system:macosx"
        links {
            "OpenGL.framework",
            "Foundation.framework",
            "Cocoa.framework",
            "glad"
        }
        sysincludedirs {
            "vendor/glad/include"
        }
        files {
            "%{prj.name}/imgui/backends/imgui_impl_osx.*"
        }
    filter "system:windows"
        links {
            "opengl32.lib"
        }
        files {
            "%{prj.name}/imgui/backends/imgui_impl_win32.*",
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
    files {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/include/**.h",
        "%{prj.name}/platform/*.h",
        "%{prj.name}/platform/%{cfg.system}/**.h",
        "%{prj.name}/platform/%{cfg.system}/**.cpp",
    }
    includedirs {
        "%{prj.name}/src",
        "%{prj.name}/include",
        "%{prj.name}/platform"
    }
    sysincludedirs {
        "vendor/assimp/include",
        "vendor/mono/include",
        "yaml/yaml-cpp/include",
        "third-party/minimp3",
        "third-party/stb",
        "third-party/json/single-include",
        "imgui/imgui",
        "imgui/extensions",
        "glm",
    }
    defines {
        "BANANA_BUILD",
    }
    links {
        "imgui",
        "yaml"
    }
    filter "system:not macosx"
        pchheader "pch.h"
        pchsource "%{prj.name}/src/pch.cpp"
        sysincludedirs {
            "vendor/glew/include",
        }
    filter "options:renderer=opengl"
        defines {
            "RENDERER_OPENGL"
        }
    filter "system:linux"
        syslibdirs {
            "/usr/lib/x86_64-linux-gnu"
        }
        links {
            "X11",
            "GLEW",
            "GLU",
            "GL",
            "assimp",
            "xcb",
            "X11-xcb"
        }
    filter "system:macosx"
        libdirs {
            "/usr/local/opt/assimp/lib"
        }
        links {
            "OpenGL.framework",
            "Foundation.framework",
            "Cocoa.framework",
            "assimp",
            "glad"
        }
        files {
            "%{prj.name}/platform/%{cfg.system}/**.mm"
        }
        sysincludedirs {
            "vendor/glad/include"
        }
    filter "system:windows"
        syslibdirs {
            "vendor/assimp/lib/%{cfg.buildcfg}/",
            "vendor/mono/lib/%{cfg.buildcfg}",
            "vendor/glew/lib/%{cfg.buildcfg}",
        }
        links {
            "opengl32.lib",
            "dinput8.lib",
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
        "%{prj.name}/platform/%{cfg.system}/**.cpp",
        "%{prj.name}/platform/%{cfg.system}/**.h",
    }
    includedirs {
        "%{prj.name}/src"
    }
    sysincludedirs {
        "thebanana/include",
        "glm",
        "vendor/glew/include",
        "imgui/imgui",
        "yaml/yaml-cpp/include",
        "imguizmo/ImGuizmo",
        "third-party/stb"
    }
    links {
        "thebanana",
        "yaml",
        "imgui",
        "sandbox",
    }
    filter "system:linux"
        syslibdirs {
            "/usr/lib/x86_64-linux-gnu",
        }
        links {
            "X11",
            "GLEW",
            "GLU",
            "GL",
        }
    filter "system:macosx"
        links {
            "OpenGL.framework",
            "Foundation.framework",
            "Cocoa.framework",
        }
        files {
            "%{prj.name}/platform/%{cfg.system}/**.mm",
        }
    filter { "system:macosx", "action:xcode*" }
        postbuildcommands {
            '{COPY} "fonts" "%{cfg.targetdir}"',
            '{COPY} "shaders" "%{cfg.targetdir}"',
        }
    filter "system:windows"
        links {
            "opengl32.lib"
        }
        postbuildcommands {
            '{COPY} "%{cfg.targetdir}/../thebanana/thebanana.dll" "%{cfg.targetdir}"',
            '{COPY} "%{cfg.targetdir}/../imgui/imgui.dll" "%{cfg.targetdir}"',
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
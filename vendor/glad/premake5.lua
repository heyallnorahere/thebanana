project "glad"
    kind "StaticLib"
    language "C"
    staticruntime "off"
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
    files {
        "src/glad.c",
        "include/glad/glad.h"
    }
    sysincludedirs {
        "include"
    }
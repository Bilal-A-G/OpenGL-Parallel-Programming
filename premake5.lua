workspace "Position Based Fluids"
    architecture "x64"

    configurations
    {
        "Debug",
        "Release"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Engine"
    location "Engine"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("int/" .. outputdir .. "/%{prj.name}")
    libdirs("%{prj.name}/lib")
    includedirs
    {
        "%{prj.name}/include",
        "external"
    }

    defines
    {
        "_CRT_SECURE_NO_WARNINGS"
    }

    pchheader "TSpch.h"
    pchsource "Engine/src/TSpch.cpp"

    postbuildcommands
    {
        ("{COPY} ../external/assimp" .. " ../bin/" .. outputdir .. "/Executables")
    }

    files
    {
        "%{prj.name}/**.h",
        "%{prj.name}/**.cpp",
        "%{prj.name}/**.c",
        "%{prj.name}/**.glsl"
    }

    links
    {
        "glfw3_mt",
        "OpenGL32",
        "assimp-vc142-mt",
    }

    filter "system:windows"
        systemversion "latest"

        defines
        {
            "TS_WIN",
            "TS_ENGINE"
        }

    filter "configurations:Debug"
        defines "TS_DEBUG"
        symbols "On"

    filter "configurations:Release"
        defines "TS_RELEASE"
        optimize "speed"

    filter "files:**.c"
        flags {"NoPCH"}

project "Demo"
    location "Demo"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    targetdir ("bin/" .. outputdir .. "/Executables")
    objdir ("int/" .. outputdir .. "/%{prj.name}")

    staticruntime "On"

    includedirs
    {
        "Engine/src",
        "Engine/include",
        "external"
    }

    files
    {
        "%{prj.name}/**.h",
        "%{prj.name}/**.cpp",
        "%{prj.name}/**.c",
        "%{prj.name}/**.glsl"
    }

    links
    {
        "Engine",
    }

    filter "system:windows"
        systemversion "latest"

        defines
        {
            "TS_WIN"
        }

    filter "configurations:Debug"
        defines "TS_DEBUG"
        symbols "On"

    filter "configurations:Release"
        defines "TS_RELEASE"
        optimize "speed"
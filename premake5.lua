workspace "Position Based Fluids"
    architecture "x64"

    configurations
    {
        "Debug",
        "Release"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Demo"
    location "Demo"
    kind "ConsoleApp"
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
    }

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
        defines "TS_DEBUG"
        symbols "On"

    filter "configurations:Release"
        defines "TS_RELEASE"
        optimize "speed"
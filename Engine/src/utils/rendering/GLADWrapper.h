#pragma once

namespace TESLA
{
    class GLADWrapper
    {
    public:
        static void OpenGLRender();
        static bool LoadGLAD(void* test);
        static void UpdateViewport(const int &width, const int &height);
    };
}

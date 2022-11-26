#pragma once
#include "Window.h"

namespace TESLA
{
    class Application
    {
    public:
        static void Start(const int& windowWidth,const int& windowHeight, const char* name);
        static void Update();
        static void Terminate();
        static void SetInputMode(int mode, int value);
        static double GetTime();
        static Window* GetWindow(){return m_window;}
    private:
        static Window* m_window;
    };
}

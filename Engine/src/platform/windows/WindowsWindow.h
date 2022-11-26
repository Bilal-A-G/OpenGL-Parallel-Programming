#pragma once
#include "../../core/Window.h"
#include "GLFW/glfw3.h"

namespace TESLA
{
    struct WindowsWindowProperties
    {
        std::string name;
        int width, height;
        bool vSync;
        GLFWwindow* window;
        
        WindowsWindowProperties(const int& width, const int& height, const char* name, const bool vSync, GLFWwindow* window)
            :vSync(vSync), width(width), height(height), name(name), window(window){}
    };
    
    class WindowsWindow : public Window
    {
    public:
        WindowsWindow(const WindowProperties& properties);
        virtual ~WindowsWindow()
        {
            glfwDestroyWindow(m_data->window);
        }

        void Update() override;
        int GetHeight() override {return m_data->height;}
        int GetWidth() override {return m_data->width; }

        bool WindowShouldClose() override;
        void SetInputMode(int mode, int value) override;

        std::string GetName() override {return m_data->name; }
        bool GetVSync() override {return m_data->vSync; }
        void SetVSync(const bool& set) override;
    private:
        WindowsWindowProperties* m_data;
    };
}

#include "TSpch.h"
#include "Application.h"
#include "Window.h"
#include "GLFW/glfw3.h"

TESLA::Window* TESLA::Application::m_window;

void TESLA::Application::Start(const int& windowWidth,const int& windowHeight, const char* name)
{
    m_window = TESLA::Window::Create({windowWidth, windowHeight, name});
}

void TESLA::Application::Update()
{
    m_window->Update();
}

void TESLA::Application::Terminate()
{
    glfwTerminate();
}

double TESLA::Application::GetTime()
{
    return glfwGetTime();
}

void TESLA::Application::SetInputMode(int mode, int value)
{
    m_window->SetInputMode(mode, value);
}





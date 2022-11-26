#include "TSpch.h"
#include "WindowsWindow.h"

#include "../../utils/events/ApplicationEvents.h"
#include "../../utils/events/KeyEvents.h"
#include "../../utils/events/MouseEvents.h"
#include "../../utils/rendering/GLADWrapper.h"

void LogGLFWErrors(int id, const char* error_message) {TS_LOG_MESSAGE(TESLA_LOGGER::ERR, "GLFW error: {0}, ID = {1}", error_message, id);}
void ResizeWindow(GLFWwindow* _, int width, int height) {TESLA::GLADWrapper::UpdateViewport(width, height);}

static bool initializedGLFW;

void TESLA::WindowsWindow::Update()
{
    glfwPollEvents();
    glfwSwapBuffers(m_data->window);
}

TESLA::Window* TESLA::Window::Create(const WindowProperties& properties)
{
    return new WindowsWindow(properties);
}

TESLA::WindowsWindow::WindowsWindow(const WindowProperties& properties)
{
    if(!initializedGLFW)
    {
        const int initialized = glfwInit();
        TS_LOG_ASSERTION(initialized == GLFW_TRUE, TESLA_LOGGER::ERR, "Failed to initialize GLFW");
        glfwSetErrorCallback(LogGLFWErrors);

        TS_LOG_MESSAGE(TESLA_LOGGER::INFO, "Successfully initialized GLFW");
        initializedGLFW = true;
    }
    
    GLFWwindow* window = glfwCreateWindow(properties.width, properties.height, properties.name.data(), nullptr, nullptr);
    glfwMakeContextCurrent(window);

    TS_LOG_ASSERTION((window != nullptr), TESLA_LOGGER::ERR, "Failed to create window");
    TS_LOG_MESSAGE(TESLA_LOGGER::INFO, "Successfully created window, width = {0}, height = {1}, name = {2}", properties.width, properties.height, properties.name);

    m_data = new WindowsWindowProperties(properties.width, properties.height, properties.name.data(), true, window);
    
    const bool gladLoaded = TESLA::GLADWrapper::LoadGLAD(glfwGetProcAddress);

    TS_LOG_ASSERTION(gladLoaded, TESLA_LOGGER::ERR, "Failed to load GLAD");
    TS_LOG_MESSAGE(TESLA_LOGGER::INFO, "Successfully loaded GLAD");
    
    TESLA::GLADWrapper::UpdateViewport(properties.width, properties.height);
    glfwSetFramebufferSizeCallback(m_data->window, ResizeWindow);

    WindowsWindow::SetVSync(true);
    
    glfwSetWindowCloseCallback(window, [](GLFWwindow* _window){EventListener::Invoke(new WindowClosedEvent());    });
    glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods){EventListener::Invoke(new MouseButtonEvent(button, action == GLFW_PRESS, action == GLFW_REPEAT)); });
    glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xpos, double ypos){EventListener::Invoke(new MouseMovedEvent(xpos,ypos)); });
    glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods){EventListener::Invoke(new KeyboardButtonEvent(key, action == GLFW_PRESS, action == GLFW_REPEAT)); });
    glfwSetScrollCallback(window, [](GLFWwindow* window, double xoffset, double yoffset){EventListener::Invoke(new MouseScrolledEvent(xoffset, yoffset));});
}

void TESLA::WindowsWindow::SetVSync(const bool& set)
{
    glfwSwapInterval(set ? 1 : 0);
    m_data->vSync = set;
}

void TESLA::WindowsWindow::SetInputMode(int mode, int value)
{
    glfwSetInputMode(m_data->window, mode, value);
}

bool TESLA::WindowsWindow::WindowShouldClose()
{
    return glfwWindowShouldClose(m_data->window);
}












#include <iostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"

constexpr int width = 800;
constexpr int height = 700;

void LogGLFWErrors(int errorCode, const char* description)
{
    std::cout << "GLFW ERROR, code = " << errorCode << ", description = " << description;
}

void ResizeWindow(GLFWwindow* _, int newWidth, int newHeight)
{
    glViewport(0, 0, newWidth, newHeight);
}

int main()
{
    glfwInit();
    glfwSetErrorCallback(LogGLFWErrors);

    GLFWwindow* window = glfwCreateWindow(width, height, "Position Based Fluids Demo", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    bool gladLoaded = gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
    std::cout << (gladLoaded ? "GLFW successfully initialized" : "GLFW failed to initialize") << "/n";
    glViewport(0, 0, width, height);

    glfwSetFramebufferSizeCallback(window, ResizeWindow);
    glfwSwapInterval(1);

    while (!glfwWindowShouldClose(window))
    {
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        glClearColor(0.1f, 0.1f, 0.1f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwTerminate();
}
#include "TSpch.h"
#include "GLADWrapper.h"
#include "glad/glad.h"

void TESLA::GLADWrapper::OpenGLRender()
{
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glClearColor(0.1f, 0.1f, 0.1f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

bool TESLA::GLADWrapper::LoadGLAD(void* test)
{
    return gladLoadGLLoader(reinterpret_cast<GLADloadproc>(test));
}

void TESLA::GLADWrapper::UpdateViewport(const int& width, const int& height)
{
    glViewport(0, 0, width, height);
}



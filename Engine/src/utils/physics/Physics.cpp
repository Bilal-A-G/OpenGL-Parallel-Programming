#include "TSpch.h"
#include "Physics.h"

#include "glad/glad.h"

uint32_t TESLA::Physics::m_computeProgram;
TESLA::Texture TESLA::Physics::m_computeBuffer;
int TESLA::Physics::m_workHeight;
int TESLA::Physics::m_workWidth;

void TESLA::Physics::Init(uint32_t computeProgram, TESLA::Texture computeBuffer, int width, int height, glm::vec4 initPosition)
{
    m_computeProgram = computeProgram;
    m_computeBuffer = computeBuffer;
    m_workHeight = height;
    m_workWidth = width;

    glUseProgram(m_computeProgram);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_computeBuffer.GetGLTexture());

    std::vector<glm::vec4> initialValues;
    initialValues.reserve(width * height);
    for(int i = 0; i < initialValues.size(); i++)
    {
        initialValues[i] = initPosition;
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, initialValues.data());
}

std::vector<glm::vec4> TESLA::Physics::GetPositionData()
{
    unsigned int size = m_workWidth * m_workHeight;
    std::vector<glm::vec4> data;
    data.reserve(size);
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_FLOAT, data.data());
    
    return data;
}

void TESLA::Physics::ComputePhysics()
{
    glUseProgram(m_computeProgram);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_computeBuffer.GetGLTexture());
    glDispatchCompute(m_workWidth, m_workHeight, 1);
    glMemoryBarrier(GL_ALL_BARRIER_BITS);
}



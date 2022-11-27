#include "TSpch.h"
#include "Physics.h"

#include "glad/glad.h"

uint32_t TESLA::Physics::m_computeProgram;
TESLA::Texture TESLA::Physics::m_computeBuffer;
int TESLA::Physics::m_workHeight;
int TESLA::Physics::m_workWidth;

void TESLA::Physics::Init(uint32_t computeProgram, TESLA::Texture computeBuffer, int width, int height, std::vector<glm::vec4> instancedPositions)
{
    m_computeProgram = computeProgram;
    m_computeBuffer = computeBuffer;
    m_workHeight = height;
    m_workWidth = width;

    glUseProgram(m_computeProgram);
    glBindTexture(GL_TEXTURE_2D, m_computeBuffer.GetGLTexture());
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, instancedPositions.data());
}

std::vector<glm::vec4> TESLA::Physics::GetPositionData()
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_computeBuffer.GetGLTexture());
    
    unsigned int size = m_workWidth * m_workHeight;
    std::vector<glm::vec4> data(size);
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



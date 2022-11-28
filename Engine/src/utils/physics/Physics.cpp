#include "TSpch.h"
#include "Physics.h"

#include "glad/glad.h"

uint32_t TESLA::Physics::m_computeProgram;
TESLA::Texture TESLA::Physics::m_computeBuffer;
int TESLA::Physics::m_workHeight;
int TESLA::Physics::m_workWidth;
uint32_t ssbox;
uint32_t ssboy;
uint32_t ssboz;

void TESLA::Physics::Init(uint32_t computeProgram, TESLA::Texture computeBuffer, int width, int height, std::vector<glm::vec4> instancedPositions)
{
    m_computeProgram = computeProgram;
    m_computeBuffer = computeBuffer;
    m_workHeight = height;
    m_workWidth = width;
    
    glUseProgram(m_computeProgram);
    glGenBuffers(1, &ssbox);
    glGenBuffers(1, &ssboy);
    glGenBuffers(1, &ssboz);

    std::vector<float> xPositions(instancedPositions.size());
    std::vector<float> yPositions(instancedPositions.size());
    std::vector<float> zPositions(instancedPositions.size());

    for(int i = 0; i < instancedPositions.size(); i++)
    {
        xPositions[i] = instancedPositions[i].x;
        yPositions[i] = instancedPositions[i].y;
        zPositions[i] = instancedPositions[i].z;
    }
    
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbox);
    glBufferData(GL_SHADER_STORAGE_BUFFER, xPositions.size() * sizeof(float), xPositions.data(), GL_DYNAMIC_DRAW);
    
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, ssboy);
    glBufferData(GL_SHADER_STORAGE_BUFFER, yPositions.size() * sizeof(float), yPositions.data(), GL_DYNAMIC_DRAW);
    
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, ssboz);
    glBufferData(GL_SHADER_STORAGE_BUFFER, zPositions.size() * sizeof(float), zPositions.data(), GL_DYNAMIC_DRAW);
}

std::vector<glm::vec4> TESLA::Physics::GetPositionData()
{
    glUseProgram(m_computeProgram);
    unsigned int size = m_workWidth * m_workHeight;
    std::vector<glm::vec4> data(size);
    
    GLfloat* ptrx;
    GLfloat* ptry;
    GLfloat* ptrz;
    
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbox);
    ptrx = reinterpret_cast<GLfloat*>(glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_WRITE));
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssboy);
    ptry = reinterpret_cast<GLfloat*>(glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_WRITE));
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssboz);
    ptrz = reinterpret_cast<GLfloat*>(glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_WRITE));
    
    for(int i = 0; i < size; i++)
    {
        data[i] = glm::vec4(ptrx[i], ptry[i], ptrz[i], 0);
    }
    
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbox);
    glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssboy);
    glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssboz);
    glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

    return data;
}

void TESLA::Physics::ComputePhysics()
{
    glUseProgram(m_computeProgram);
    glDispatchCompute(m_workWidth * m_workHeight, 1, 1);
}



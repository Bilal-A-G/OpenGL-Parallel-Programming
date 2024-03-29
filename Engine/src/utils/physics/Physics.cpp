﻿#include "TSpch.h"
#include "Physics.h"
#include "glad/glad.h"

uint32_t TESLA::Physics::m_computeProgram;
int TESLA::Physics::m_size;

int TESLA::Physics::m_subSteps = 10;
float TESLA::Physics::m_deltaTime = 1.0f/60.0f;

std::vector<glm::vec4> TESLA::Physics::m_data;

std::vector<uint32_t> cellStart;
std::vector<uint32_t> cellEntries;
std::vector<uint32_t> queryIDs;

uint32_t ssbox;
uint32_t ssboy;
uint32_t ssboz;
uint32_t ssbovel;
uint32_t ssbocellentries;
uint32_t ssbocellstart;
uint32_t ssboqueryids;

int tableSize;
float spacing = 3;
float querySize = 0;

int GetHashCoords(glm::vec4 tabCoords)
{
    int h = (int)tabCoords.x * 92837111 ^ (int)tabCoords.y * 689287499 ^ (int)tabCoords.z * 283923481;
    return abs(h) % tableSize;
}

glm::vec4 GetTableCoords(float x, float y, float z)
{
    return glm::vec4(floor(x / spacing), floor(y / spacing), floor(z / spacing), 0);
}

void TESLA::Physics::Init(uint32_t computeProgram, int size, std::vector<glm::vec4> instancedPositions)
{
    m_computeProgram = computeProgram;
    m_size = size;
    m_data = instancedPositions;
    tableSize = instancedPositions.size() * 2;
    queryIDs = std::vector<uint32_t>(instancedPositions.size());
    
    glUseProgram(m_computeProgram);
    glGenBuffers(1, &ssbox);
    glGenBuffers(1, &ssboy);
    glGenBuffers(1, &ssboz);
    glGenBuffers(1, &ssbovel);
    glGenBuffers(1, &ssbocellentries);
    glGenBuffers(1, &ssbocellstart);
    glGenBuffers(1, &ssboqueryids);

    std::vector<float> xPositions(instancedPositions.size());
    std::vector<float> yPositions(instancedPositions.size());
    std::vector<float> zPositions(instancedPositions.size());

    for(int i = 0; i < instancedPositions.size(); i++)
    {
        xPositions[i] = instancedPositions[i].x;
        yPositions[i] = instancedPositions[i].y;
        zPositions[i] = instancedPositions[i].z;
    }

    std::vector<glm::vec4> initialVelocities(instancedPositions.size());

    for(int i = 0; i < initialVelocities.size(); i++)
    {
        initialVelocities[i] = glm::vec4(rand() % 2, rand() % 2, rand() % 2, 0);
    }

    float scaledDeltaTime = m_deltaTime/m_subSteps;
    GLuint deltaTimeLocation = glGetUniformLocation(m_computeProgram, "deltaTime");
    glUniform1f(deltaTimeLocation, scaledDeltaTime);

    GLuint numParticlesLocation = glGetUniformLocation(m_computeProgram, "numParticles");
    glUniform1i(numParticlesLocation, instancedPositions.size());

    GLuint spacingLocation = glGetUniformLocation(m_computeProgram, "spacing");
    glUniform1f(spacingLocation, spacing);

    GLuint tableSizeLocation = glGetUniformLocation(m_computeProgram, "tableSize");
    glUniform1i(tableSizeLocation, tableSize);
    
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbox);
    glBufferData(GL_SHADER_STORAGE_BUFFER, xPositions.size() * sizeof(float), xPositions.data(), GL_DYNAMIC_DRAW);
    
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, ssboy);
    glBufferData(GL_SHADER_STORAGE_BUFFER, yPositions.size() * sizeof(float), yPositions.data(), GL_DYNAMIC_DRAW);
    
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, ssboz);
    glBufferData(GL_SHADER_STORAGE_BUFFER, zPositions.size() * sizeof(float), zPositions.data(), GL_DYNAMIC_DRAW);

    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, ssbovel);
    glBufferData(GL_SHADER_STORAGE_BUFFER, initialVelocities.size() * sizeof(glm::vec4), initialVelocities.data(), GL_DYNAMIC_DRAW);

    std::vector<uint32_t> cellStart(tableSize + 1);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 5, ssbocellstart);
    glBufferData(GL_SHADER_STORAGE_BUFFER, cellStart.size() * sizeof(uint32_t), cellStart.data(), GL_DYNAMIC_DRAW);

    std::vector<uint32_t> cellEntries(size);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 6, ssbocellentries);
    glBufferData(GL_SHADER_STORAGE_BUFFER, cellEntries.size() * sizeof(uint32_t), cellEntries.data(), GL_DYNAMIC_DRAW);

    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 7, ssboqueryids);
    glBufferData(GL_SHADER_STORAGE_BUFFER, queryIDs.size() * sizeof(uint32_t), queryIDs.data(), GL_DYNAMIC_DRAW);
}

std::vector<glm::vec4>& TESLA::Physics::GetPositionData()
{
    cellStart = std::vector<uint32_t>(tableSize + 1);
    cellEntries = std::vector<uint32_t>(m_size);
    queryIDs = std::vector<uint32_t>(m_size);
    
    GLfloat* ptrx;
    GLfloat* ptry;
    GLfloat* ptrz;

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbox);
    ptrx = reinterpret_cast<GLfloat*>(glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_WRITE));
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssboy);
    ptry = reinterpret_cast<GLfloat*>(glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_WRITE));
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssboz);
    ptrz = reinterpret_cast<GLfloat*>(glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_WRITE));
    
    //TODO: Compute all hash table related things in separate compute shaders (one for each for loop) and pass them via ssbos to each other and the physics compute shader
    for(int i = 0; i < m_size; i++)
    { 
        m_data[i] = glm::vec4(ptrx[i], ptry[i], ptrz[i], 0);
        int index = GetHashCoords(GetTableCoords(ptrx[i], ptry[i], ptrz[i]));
        cellStart[index]++;
    }

    int cumSum = 0;
    for(int i = 0; i < tableSize; i++)
    {
        cumSum += cellStart[i];
        cellStart[i] = cumSum;
    }
    cellStart[tableSize] = cumSum;

    for(int i = 0; i < m_size; i++)
    {
        int index = GetHashCoords(GetTableCoords(ptrx[i], ptry[i], ptrz[i]));
        cellStart[index]--;
        cellEntries[cellStart[index]] = i;
    }
    
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbocellstart);
    glBufferData(GL_SHADER_STORAGE_BUFFER, cellStart.size() * sizeof(uint32_t), cellStart.data(), GL_DYNAMIC_DRAW);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbocellentries);
    glBufferData(GL_SHADER_STORAGE_BUFFER, cellEntries.size() * sizeof(uint32_t), cellEntries.data(), GL_DYNAMIC_DRAW);

    glMemoryBarrier(GL_ALL_BARRIER_BITS);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbox);
    glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
    
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssboy);
    glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
    
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssboz);
    glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

    return m_data;
}

void TESLA::Physics::ComputePhysics()
{
    for(int  i = 0; i < m_subSteps; i++)
    {
        glUseProgram(m_computeProgram);
        glDispatchCompute(m_size, 1, 1);
        glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
    }
}
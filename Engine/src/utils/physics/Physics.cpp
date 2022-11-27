#include "TSpch.h"
#include "Physics.h"

#include "glad/glad.h"

uint32_t TESLA::Physics::m_computeProgram;

void TESLA::Physics::Init(uint32_t computeProgram)
{
    m_computeProgram = computeProgram;
}

void TESLA::Physics::ComputePhysics(int width, int height, int depth)
{
    glUseProgram(m_computeProgram);
    glDispatchCompute(width, height, depth);
    glMemoryBarrier(GL_ALL_BARRIER_BITS);
}



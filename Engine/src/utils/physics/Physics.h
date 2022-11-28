﻿#pragma once
#include <glm/vec4.hpp>
#include "../rendering/Texture.h"

namespace TESLA
{
    class Physics
    {
    public:
        static void Init(uint32_t computeProgram, TESLA::Texture computeBuffer, int width, int height, std::vector<glm::vec4> instancePositions);
        static void ComputePhysics();
        static std::vector<glm::vec4> GetPositionData();
    private:
        static uint32_t m_computeProgram;
        static TESLA::Texture m_computeBuffer;
        static int m_workHeight;
        static int m_workWidth;
        
        static int m_subSteps;
        static float m_deltaTime;
    };    
}


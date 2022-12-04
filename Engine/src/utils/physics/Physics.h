#pragma once
#include <glm/vec4.hpp>
#include "../rendering/Texture.h"

namespace TESLA
{
    class Physics
    {
    public:
        static void Init(uint32_t computeProgram, int size, std::vector<glm::vec4> instancePositions);
        static void ComputePhysics();
        static std::vector<glm::vec4>& GetPositionData();
    private:
        static uint32_t m_computeProgram;
        static int m_size;
        
        static int m_subSteps;
        static float m_deltaTime;
        static std::vector<glm::vec4> m_data;
    };    
}


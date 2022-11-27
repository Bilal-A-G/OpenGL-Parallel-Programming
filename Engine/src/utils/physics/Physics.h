#pragma once

namespace TESLA
{
    class Physics
    {
    public:
        static void Init(uint32_t computeProgram);
        static void ComputePhysics(int width, int height, int depth);
    private:
        static uint32_t m_computeProgram;
    };    
}


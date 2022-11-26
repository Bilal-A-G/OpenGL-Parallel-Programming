#pragma once
#include <glm/glm.hpp>

class Camera
{
public:
    static void Init();
    inline static glm::vec3 cameraPosition = glm::vec3(1.5f, 1.0f, 1.5f);
    inline static glm::vec3 cameraDirection;
    static glm::mat4 CalculateView();
};

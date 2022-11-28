#version 460 core
layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
layout(std430, binding = 0) buffer positionsx {float xPositions[];};
layout(std430, binding = 1) buffer positionsy {float yPositions[];};
layout(std430, binding = 2) buffer positionsz {float zPositions[];};

void main()
{
    uint pos = gl_GlobalInvocationID.x;
    float newY = yPositions[pos] - 0.1;
    yPositions[pos] = newY;
}
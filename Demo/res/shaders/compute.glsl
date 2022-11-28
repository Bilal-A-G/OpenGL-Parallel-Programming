#version 460 core
layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
layout(std430, binding = 0) buffer positionsx {float xPositions[];};
layout(std430, binding = 1) buffer positionsy {float yPositions[];};
layout(std430, binding = 2) buffer positionsz {float zPositions[];};

layout(std430, binding = 3) buffer vel {vec4 velocities[];};
layout(std430, binding = 4) buffer prvpos {vec4 prevPositions[];};

uniform float deltaTime;

void main()
{
    uint pos = gl_GlobalInvocationID.x;
    velocities[pos] = velocities[pos] + vec4(0, -9.8, 0, 0) * deltaTime;
    prevPositions[pos] = vec4(xPositions[pos], yPositions[pos], zPositions[pos], 0);
    
    xPositions[pos] = xPositions[pos] + velocities[pos].x * deltaTime;
    yPositions[pos] = yPositions[pos] + velocities[pos].y * deltaTime;
    zPositions[pos] = zPositions[pos] + velocities[pos].z * deltaTime;
    
    velocities[pos] = (vec4(xPositions[pos], yPositions[pos], zPositions[pos], 0) - prevPositions[pos])/deltaTime;
}
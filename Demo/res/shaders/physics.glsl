#version 460 core
layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
layout(std430, binding = 0) buffer positionsx {float xPositions[];};
layout(std430, binding = 1) buffer positionsy {float yPositions[];};
layout(std430, binding = 2) buffer positionsz {float zPositions[];};

layout(std430, binding = 3) buffer vel {vec4 velocities[];};
layout(std430, binding = 4) buffer prvpos {vec4 predictedPositions[];};
layout(std430, binding = 8) buffer lambda {vec4 lambdas[];};

layout(std430, binding = 5) buffer cellst {int cellStart[];};
layout(std430, binding = 6) buffer cellen {int cellEntries[];};
layout(std430, binding = 7) buffer qids {int queryIDs[];};

uniform float deltaTime;
uniform float spacing;
uniform int numParticles;
uniform int tableSize;

int GetHashCoords(float x, float y, float z)
{
    int h = int(x) * 92837111 ^ int(y) * 689287499 ^ int(z) * 283923481;
    return abs(h) % tableSize;
}

vec3 GetTableCoords(float x, float y, float z)
{
    return vec3(floor(x / spacing), floor(y / spacing), floor(z / spacing));
}

vec3 PolySix()

int querySize = 0;

void Query(vec4 position, float maxDist, float pos)
{
    vec3 p0 = GetTableCoords(position.x - maxDist, position.y - maxDist, position.z - maxDist);
    vec3 p1 = GetTableCoords(position.x + maxDist, position.y + maxDist, position.z + maxDist);
    querySize = 0;
    
    for(int xi = int(p0.x); xi < int(p1.x); xi++)
    {
        for(int yi = int(p0.y); yi < int(p1.y); yi++)
        {
            for(int zi = int(p0.z); zi < int(p1.z); zi++)
            {
                int hashCoord = GetHashCoords(xi, yi, zi);
                int start = cellStart[hashCoord];
                int end = cellStart[hashCoord + 1];
                                
                for(int i = start; i < end; i++)
                {
                    queryIDs[querySize] = cellEntries[i];
                    querySize++;
                }
            }
        }
    }
    
}

void main()
{
    float radius = 2;
    
    uint pos = gl_GlobalInvocationID.x;
    vec4 currentPosition = vec4(xPositions[pos], yPositions[pos], zPositions[pos], 0);
    
    velocities[pos] = velocities[pos] + vec4(0, -9.8, 0, 0) * deltaTime;
    predictedPositions[pos] = currentPosition + velocities[pos] * deltaTime;
    
    Query(currentPosition, 2, pos);
    
    //Calculate lambda
    
    barrier();
    //Calculate delta p
    vec4 deltaP;
    
    //Collision responses
    
    //Update predicted positions
    predictedPositions[pos] = predictedPositions[pos] + deltaP;
    
    //Update velocity
    velocities[pos] = 1/deltaTime * (predictedPositions[pos] - currentPosition);
    
    //Vorticity confinement and XSPH viscosity
    
    //Update positions
    xPositions[pos] = predictedPositions[pos].x;
    yPositions[pos] = predictedPositions[pos].y;
    zPositions[pos] = predictedPositions[pos].z;
}

    //for(int i = 0; i < querySize; i++)
    //{
        //int j = queryIDs[i];
        //float dist = distance(vec3(xPositions[pos], yPositions[pos], zPositions[pos]), vec3(xPositions[j], yPositions[j], zPositions[j]));
        //if(dist > 0 && dist < radius)
        //{
            //vec3 correction = normalize(vec3(xPositions[pos], yPositions[pos], zPositions[pos]) - vec3(xPositions[j], yPositions[j], zPositions[j]));
            //velocities[pos] = vec4(correction.xyz, 0) * 4;
        //}
    //}
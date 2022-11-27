#version 460 core
#extension GL_ARB_separate_shader_objects : enable
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;
layout (location = 3) in vec4 translation;

uniform mat4 view;
uniform mat4 projection;

out vec3 outPosition;
out vec3 outNormal;
out vec2 outUv;

void main()
{
    mat4 model;
    model[0][0] = 1;
    model[1][1] = 1;
    model[2][2] = 1;
    model[3] = vec4(translation.xyz, 1);
    
    gl_Position = (projection * view * model) * vec4(position.xyz, 1);
    //outNormal = mat3(transpose(inverse(model))) * normal;
    //outPosition = vec3(model * vec4(position.xyz, 1));
    //outUv = uv;
}
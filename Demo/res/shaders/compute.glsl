#version 460 core
layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
layout(rgba32f, binding = 0) uniform image2D m_buffer;

void main()
{
    ivec2 pos = ivec2( gl_GlobalInvocationID.xy );
    float x = imageLoad(m_buffer, pos).r;
    float y = imageLoad(m_buffer, pos).g;
    float z = imageLoad(m_buffer, pos).b;
    
    imageStore(m_buffer, pos, vec4(x, y, z, 0.0));
}
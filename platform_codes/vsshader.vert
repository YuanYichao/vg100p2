#version 450 core
layout (location = 0) in vec3 aPos;
uniform mat4x4 world;
uniform mat4x4 proj;
uniform mat4x4 camera;
out vec3 pos;
void main()
{
    gl_Position = proj * camera * world * vec4(aPos, 1.0);
    pos = aPos;
}

#version 330 core
out vec4 FragColor;
in vec3 pos;
void main()
{
    FragColor =  vec4(1.0f , 0.5f - pos.z/2.0f , 0.5f - pos.z/2.0f , 1.0f);
};

#version 330 core
layout (location = 0) in vec3 aPos;
uniform mat4 mvMatrix;
uniform mat4 projMatrix;
uniform mat4 camMatrix;
uniform float t;
out vec3 pos;
out float a;
float func(vec2 xy){
    return 1 / xy.y;
}

void main()
{
    float z = func(aPos.xy);
    a = 1.0f;
    if(abs(z) > 2.0f){
        a = 0.0f;
    }
    gl_Position = projMatrix * camMatrix *  mvMatrix * vec4(aPos.xy, z, 1.0);
    pos = vec3(aPos.xy, z);
}

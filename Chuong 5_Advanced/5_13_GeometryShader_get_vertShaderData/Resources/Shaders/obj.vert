#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 aColor;
out VS_OUT{vec3 color;} vs_out;
void main()
{
    gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
    gl_PointSize=20;
    vs_out.color= aColor;
}

#version 460 core

in vec3 vPos;
uniform float mX;
uniform float mY;
out vec4 FragColor;


void main()
{
    float red = mX;
    float green = mY;
    FragColor = vec4(red, green, 1.0, 1.0);
}
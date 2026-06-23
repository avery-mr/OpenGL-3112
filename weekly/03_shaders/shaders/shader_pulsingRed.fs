#version 460 core

out vec4 FragColor;
uniform float uTime;

void main()
{
    float redVal = 0.5 + 0.5 * sin(uTime * 2.0);
    FragColor = vec4(redVal, 0.0, 0.0, 1.0);
}
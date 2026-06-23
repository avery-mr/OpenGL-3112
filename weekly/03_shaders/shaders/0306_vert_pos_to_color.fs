#version 460 core

in vec3 vPos;
out vec4 FragColor;


void main()
{
    vec3 newColor = vec3(vPos.x * 0.5 + 0.5, vPos.y * 0.5 + 0.5, 0.0);
    FragColor = vec4(newColor, 1.0);
}
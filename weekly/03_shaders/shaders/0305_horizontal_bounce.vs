#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

uniform float uTime;

out vec3 ourColor;
void main()
{
	vec3 newPos = (aPos.x + sin(uTime) * 0.5, aPos.y, aPos.z);
	gl_Position = vec4(newPos, 1.0);
	ourColor = aColor;
}
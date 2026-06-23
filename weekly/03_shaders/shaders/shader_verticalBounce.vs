#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;


uniform float uTime;

void main()
{
	vec3 newPos = vec3(aPos.x, aPos.y + sin(uTime * 2.0) * 0.5, aPos.z);
	gl_Position = vec4(newPos, 1.0);  
}
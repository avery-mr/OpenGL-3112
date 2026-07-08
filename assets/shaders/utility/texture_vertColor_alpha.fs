#version 460 core

out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D ourTexture;

void main()
{
	float alpha = ourColor.x; // use the red channel of the ver
	FragColor = texture(ourTexture, TexCoord) * vec4(1.0f, 1.0f, 1.0f, alpha);
}
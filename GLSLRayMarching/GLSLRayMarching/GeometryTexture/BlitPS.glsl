#version 330 core

in vec4 color;

uniform vec2 iResolution;

out vec4 FragColor;

void main()
{
	FragColor = color;
}
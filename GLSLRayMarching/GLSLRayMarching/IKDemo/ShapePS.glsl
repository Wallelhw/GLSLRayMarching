#version 430 core

in vec2 texcoord0;
in vec4 color;


out vec4 FragColor;

void main()
{
	FragColor = color;
}
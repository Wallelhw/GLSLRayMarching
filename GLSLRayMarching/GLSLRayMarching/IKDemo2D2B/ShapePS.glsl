#version 430 core

in vec2 texcoord0;
in vec4 outcolor;


out vec4 FragColor;

void main()
{
	FragColor = outcolor;
}
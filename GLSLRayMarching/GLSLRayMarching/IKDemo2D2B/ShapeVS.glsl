#version 430 core
layout (location = 0) in vec3 vertex;

/*
layout (std140) uniform Example
{
					// alignedsize offset
	bool b;			// 4  0
	int i;			// 4  4
	float f;		// 4  8
	vec2 v2;		// 8  12
	vec3 v3;		// 16 20 
	vec4 v4;		// 16 36
	float v[3];		// 16 42   v[0]
					// 16 58   v[1]
					// 16 64   v[2]
	mat4 m;			// 16 112  column 0 of m
	        		// 16 128  column 1 of m
					// 16 144  column 2 of m
					// 16 160  column 3 of m
}; 

struct Vertex4
{
	float px[4]; // position x
    float py[4]; // position y
};
layout(std430, binding=2) buffer VertexData
{
	Vertex4 vertices[1];
};
*/

#define USE_UNIFORM_BLOCK
#ifdef USE_UNIFORM_BLOCK
layout (std140, binding=0) uniform UniformBlockData
{
	uniform mat4 worldTransform;
	uniform mat4 viewTransform;
	uniform mat4 projTransform;
	uniform vec4 color;
};  
#else
	uniform mat4 worldTransform;
	uniform mat4 viewTransform;
	uniform mat4 projTransform;	
	uniform vec4 color;
#endif

out vec2 texcoord0;
out vec4 outcolor;

void main()
{
	gl_Position = projTransform * viewTransform * worldTransform * vec4(vertex, 1.0);

	//outcolor = vec4(1.0, 1.0, 1.0, 1.0);
	outcolor = color;
}
#version 430 core
layout (location = 0) in vec2 uvs;

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
*/

#define USE_UNIFORM_BLOCK

#ifdef USE_UNIFORM_BLOCK
layout (std140, binding=0) uniform TransformData
{
	uniform mat4 viewTransform;
	uniform mat4 projTransform;
	uniform int lod;
};  
#else
	uniform mat4 viewTransform;
	uniform mat4 projTransform;	
	uniform int lod;
#endif

uniform mat4 worldTransform;
uniform sampler2D geometryTexture;
uniform sampler2D normalTexture;
uniform float ratio;
out vec4 color;

struct Vertex4
{
	float px[4]; // position x
    float py[4]; // position y
};
layout(std430, binding=2) buffer VertexData
{
	Vertex4 vertices[];
};

vec3 blend(vec3 a, vec3 b, float t)
{
	return mix(a, b, t);
}

void main()
{
	vec2 geometryTextureSize = vec2(textureSize(geometryTexture, lod));

	float x = mod(gl_InstanceID, geometryTextureSize.x);
	float y = (gl_InstanceID / geometryTextureSize.y);

	vec2 uv = (uvs + vec2(x, y)) / (geometryTextureSize.xy);
	vec4 position = textureLod(geometryTexture, uv, lod);
	vec4 normal = normalize((textureLod(normalTexture, uv, lod) - 0.5) * 2.0);

	vec4 pos;
	vec3 modelVertex = (position.xyz - 0.5);
	vec3 sheetVertex = vec3((uv.x-0.5), 0.0, (uv.y-0.5))*20.0;
	pos.xyz = blend(modelVertex, sheetVertex, ratio);
	pos.x += vertices[0].px[0];
	pos.y += vertices[0].px[1];
	pos.z += vertices[0].px[2];
	pos.w = 1.0;

	//vertices[0].px[0] += 0.001;//0.1 * gl_VertexID;

	gl_Position = projTransform * viewTransform * worldTransform * pos;

	vec4 ambientLightColor = vec4(0.2, 0.2, 0.2, 1.0);
	vec4 lightcolor = vec4(1.0, 1.0, 1.0, 1.0);
	vec3 lightDir = normalize(vec3(1.0, 1.0, 1.0));

	vec4 diffuseMaterial = vec4(0.8, 0.8, 0.8, 1.0);
	
	color = lightcolor * max(dot(lightDir, normal.xyz), 0) + ambientLightColor;
	color *= diffuseMaterial;
	
	//color = normal;
	//color = vec4(uv.xy, 0.0, 1.0);
	color.w = 1.0;
}
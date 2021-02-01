#version 430 core
layout (location = 0) in vec2 uvs;

uniform mat4 worldTransform;
uniform mat4 viewTransform;
uniform mat4 projTransform;

uniform sampler2D geometryTexture;
uniform sampler2D normalTexture;
uniform int lod;

out vec4 color;

struct Vertex4
{
	float px[4]; // position x
    float py[4]; // position y
};

layout(std430, binding=0) buffer VertexData
{
	Vertex4 vertices[];
};

void main()
{
	vec2 geometryTextureSize = vec2(textureSize(geometryTexture, lod));

	float x = mod(gl_InstanceID, geometryTextureSize.x);
	float y = (gl_InstanceID / geometryTextureSize.y);

	vec2 uv = (uvs + vec2(x, y)) / (geometryTextureSize.xy);
	vec4 position = textureLod(geometryTexture, uv, lod);
	vec4 normal = normalize((textureLod(normalTexture, uv, lod) - 0.5) * 2.0);

	vec4 pos;
	pos.xyz = (position.xyz - 0.5);
	pos.x += vertices[0].px[0];
	pos.y += vertices[0].px[1];
	pos.z += vertices[0].px[2];
	pos.w = 1.0;

	// vertices[0].px[0] += 0.1;//0.1 * gl_VertexID;

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
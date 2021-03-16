#version 430 core
layout (location = 0) in vec4 vertex;

#define USE_UNIFORM_BLOCK
#ifdef USE_UNIFORM_BLOCK
layout (std140, binding=0) uniform TransformData
{
	uniform mat4 viewTransform;
	uniform mat4 projTransform;
};  
#else
	uniform mat4 viewTransform;
	uniform mat4 projTransform;	
#endif

uniform mat4 worldTransform;

uniform ivec2 offset;
uniform ivec2 size;
uniform float alpha;

uniform sampler2D heightMap;
uniform int heightMapSize;
uniform int splatMapSize;

out vec2 texcoord0;
out vec2 texcoord1;
out vec4 color;

void main()
{
	vec2 vertexWorld = ((vertex.xy + vertex.zw * alpha) * size.xy * 0.2 + offset.xy);
	vec4 height = texture(heightMap, vertexWorld / heightMapSize);

	texcoord0 = vertexWorld.xy;
	texcoord1 = vertexWorld.xy / splatMapSize;

	gl_Position = projTransform * viewTransform * worldTransform * vec4(vertexWorld.x, height.x*100, vertexWorld.y, 1.0);

	vec4 ambientLightColor = vec4(0.2, 0.2, 0.2, 1.0);
	vec4 lightcolor = vec4(1.0, 1.0, 1.0, 1.0);
	vec3 lightDir = normalize(vec3(1.0, 1.0, 1.0));
	vec4 diffuseMaterial = vec4(0.8, 0.8, 0.8, 1.0);
	
	color = lightcolor * max(dot(lightDir, vec3(0.0, 1.0, 0.0)), 0) + ambientLightColor;
	color *= diffuseMaterial;
}
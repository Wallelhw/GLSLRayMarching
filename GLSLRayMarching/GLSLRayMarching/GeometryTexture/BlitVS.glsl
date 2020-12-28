#version 330 core
layout (location = 0) in float vindices;

uniform mat4 worldTransform;
uniform mat4 viewTransform;
uniform mat4 projTransform;

uniform sampler2D geometryTexture;
uniform vec2 geometryTextureSize;

uniform sampler2D normalTexture;
uniform vec2 normalTextureSize;

out vec4 color;

void main()
{
	//vec2 size = vec2(textureSize(geometryTexture, 0));
	vec2 size = geometryTextureSize;

	float x = mod(vindices, size.x) / size.x;
	float y = (vindices / size.y) / size.y;
	vec2 uv = vec2(x, y);
	vec4 position = textureLod(geometryTexture, uv, 0);
	vec4 normal = textureLod(normalTexture, uv, 0);

	vec4 pos;
	pos.xyz = (position.xyz - 0.5);
	pos.w = 1.0;
	gl_Position = projTransform * viewTransform * worldTransform * pos;

	vec4 ambientColor = vec4(0.2, 0.2, 0.2, 1.0);
	vec4 lightcolor = vec4(0.6, 0.6, 0.6, 1.0);
	vec3 light = normalize(vec3(1.0, 1.0, 1.0));
	
	color = lightcolor * dot(light, normal.xyz) + ambientColor;
}
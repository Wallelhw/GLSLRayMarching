#version 330 core
layout (location = 0) in vec2 uvs;

uniform mat4 worldTransform;
uniform mat4 viewTransform;
uniform mat4 projTransform;

uniform sampler2D geometryTexture;
uniform sampler2D normalTexture;
uniform int start;

out vec4 color;

void main()
{
	vec2 geometryTextureSize = vec2(textureSize(geometryTexture, 0));

	float id = gl_InstanceID + start;
	float x = mod(id, geometryTextureSize.x);
	float y = (id / geometryTextureSize.y);

	vec2 uv = (uvs + vec2(x, y)) / geometryTextureSize.xy;
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
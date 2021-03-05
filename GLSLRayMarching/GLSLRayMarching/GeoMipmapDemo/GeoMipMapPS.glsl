#version 430 core

in vec2 texcoord0;
in vec2 texcoord1;
in vec4 color;

uniform sampler2D texture0;
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform sampler2D texture3;
uniform sampler2D splatMap;

out vec4 FragColor;

void main()
{
	vec4 t0 = texture(texture0, texcoord0);
	vec4 t1 = texture(texture1, texcoord0);
	vec4 t2 = texture(texture2, texcoord0);
	vec4 t3 = texture(texture3, texcoord0);
	vec4 s = texture(splatMap, texcoord1);

	FragColor = (t0 * s.r + t1 * s.g + t2 * s.b) / (s.r + s.g + s.b);
	//FragColor = (t0 * s.r + t1 * s.g + t2 * s.b);
	//FragColor = s;
	//FragColor = vec4(1.0, 1.0 ,1.0, 1.0);
}
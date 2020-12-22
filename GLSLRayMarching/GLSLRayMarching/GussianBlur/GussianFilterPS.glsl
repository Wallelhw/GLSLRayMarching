#version 330 core
in vec2 screenCoord;

uniform vec2 screenSize;
uniform sampler2D frameBufferTexture;

uniform bool horizontal;

out vec4 FragColor;

uniform float kernel[16];
uniform int kernelSize;

vec3 guassian(bool horizontal)
{
    vec2 offset = 1.0 / screenSize;

    vec3 col = kernel[0] * texture(frameBufferTexture, screenCoord).xyz;
    if(horizontal)
    {
        for(int i=1; i<kernelSize; i++)
        {
            col += kernel[i] * texture(frameBufferTexture, screenCoord + vec2(offset.x * i, 0.0)).xyz;
            col += kernel[i] * texture(frameBufferTexture, screenCoord - vec2(offset.x * i, 0.0)).xyz;
        }
    }
    else
    {
        for(int i=1; i<kernelSize; i++)
        {
            col += kernel[i] * texture(frameBufferTexture, screenCoord + vec2(0.0, offset.y * i)).xyz;
            col += kernel[i] * texture(frameBufferTexture, screenCoord - vec2(0.0, offset.y * i)).xyz;
        }
    }

    return col;
}

void main()
{
    FragColor = vec4(guassian(horizontal), 1.0);
}

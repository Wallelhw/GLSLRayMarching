#version 330 core
in vec2 screenCoord;

uniform vec2 screenSize;
uniform sampler2D frameBufferTexture;

uniform bool horizontal;

out vec4 FragColor;

uniform int kernelSize;
uniform float colorSigma;
uniform float spaceSigma;

float I(vec2 offset)
{
    vec3 tex = texture(frameBufferTexture, screenCoord + vec2(offset.x, offset.y)).xyz;

    return tex.r * 0.3 + tex.g * 0.6 + 0.1 * tex.b;
}

float dI(vec2 offset)
{
    return I(vec2(0.0, 0.0)) - I(offset);
}

float Gs(float x)
{
    return exp(-(x * x) / (2 * spaceSigma * spaceSigma)) / sqrt(2 * 3.1415 * spaceSigma * spaceSigma);
}

float Gr(float x)
{
    return exp(-(x * x) / (2 * colorSigma * colorSigma)) / sqrt(2 * 3.1415 * colorSigma * colorSigma);
}

vec3 bilateral(bool horizontal)
{
    /*
    vec2 offset = 1.0 / screenSize;
    vec2 q;

    q = vec2(0.0, 0.0);
    vec3 col = Gs(0) * I(q);
    if(horizontal)
    {
        for(int i=1; i<kernelSize; i++)
        {
            q = vec2(offset.x * i, 0.0);
            col += Gs(float(i)) * Gc(dI(q)) * I(q);
            
            q = vec2(-offset.x * i, 0.0);
            col += Gs(float(i)) * Gc(dI(q)) * I(q);
        }
    }
    else
    {
        for(int i=1; i<kernelSize; i++)
        {
            q = vec2(0.0, offset.y * i);
            col += Gs(float(i)) * Gc(dI(q)) * I(q);
            
            q = vec2(0.0, -offset.y * i);
            col += Gs(float(i)) * Gc(dI(q)) * I(q);
        }
    }
    */

    vec2 offset = 1.0 / screenSize;
    float col = 0.0;
    float Wp = 0.0;
    for(int y=-kernelSize; y<kernelSize; y++)
    {
        for(int x=-kernelSize; x<kernelSize; x++)
        {
            vec2 q = vec2(offset.x * x, offset.y * y);
            float weight = Gs(sqrt(x*x+y*y)) * Gr(abs(dI(q)));
            col += weight * I(q);
            Wp += weight;
        }
    }

    return vec3(col, col, col) / Wp;
}

void main()
{
    FragColor = vec4(bilateral(horizontal), 1.0);
}

#define PI 3.142
#define saturate(x) clamp(x, 0.0, 1.0)


mat2 rotate(float angle)
{
	angle *= PI / 180.0;
    float s = sin(angle), c = cos(angle);
    return mat2( c, -s, s, c );
}



float hash21(vec2 p)
{
 	p = fract( p*vec2(123.34, 456.21) );
    p += dot(p, p+45.32);
    return fract(p.x*p.y);
}

vec3 hash23( vec2 co )
{
   vec3 a = fract( cos( co.x*8.3e-3 + co.y )*vec3(1.3e5, 4.7e5, 2.9e5) );
   vec3 b = fract( sin( co.x*0.3e-3 + co.y )*vec3(8.1e5, 1.0e5, 0.1e5) );
   vec3 c = mix(a, b, 0.5);
   return c;
}




float star(vec2 uv, float time, float flare)
{
	float d = length(uv);
    float m = (max(0.2, abs(sin(time))) * 0.02) / d;
    
    float rays = max(0., 1.-abs(uv.x*uv.y*1000.));
    m += rays*flare;
    uv *= rotate(45.0);
    rays = max(0., 1.-abs(uv.x*uv.y*1000.));
    m += rays*0.5*flare;
    
    m *= smoothstep(1.0, 0.2, d);
    
    return m;
}



float starFieldMin(vec2 p)
{
    vec3 rnd = hash23(p * iResolution.x);
    float intensity = pow((1.+sin((iTime+27.0)*rnd.x))*.5, 7.) ;
    return max(rnd.x * pow(rnd.y,7.) * intensity, 0.);

}


vec3 starField(vec2 uv)
{
    vec3 col = vec3(0);
    
    vec2 gv = fract(uv) - 0.5;
    vec2 id = floor(uv);
    
    for (int x=-1; x<=1; x++){
        for (int y=-1; y<=1; y++)
        {
            vec2 offset = vec2(x, y);
            
            float n = hash21(id + offset);
            float size = min(0.25, fract(n*1234.567) + 0.1);
            float star = star(gv - offset - (vec2(n, fract(n*100.0)) - 0.5), iTime*fract(n*135.246), smoothstep(.9, 1., size)*.6);
            
            col += star * size;
        }
    }
    
    return col;
}




float wave(vec2 p)
{
    return 1.0-abs( p.y+sin(p.x) );
}


float terrain(vec2 uv)
{
    float d = 0.0;
    float x = uv.x*2.0, f = 0.8, a = 0.05;
    for (int p=0; p<5; p++){
        d += sin(x * f) * a;
        f *= 2.0;
        a *= 0.5;
    }
    d = abs(d);
    return sign((uv.y+d)-0.1);
}




const vec3 purple = vec3(0.318,0.192,0.369);
const vec3 green = vec3(0.41, 0.86, 0.6);


void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    vec2 uv = fragCoord.xy/min(iResolution.x, iResolution.y);
    vec2 uvS = fragCoord.xy/iResolution.xy;
    
    vec2 uvR = uv * rotate(mod(iTime*0.4, 360.0));
    //uvR.y += iTime*0.01;
    
    vec3 color;
    
    color += (starField(uvR*50.0)) + starFieldMin(uv)*2.0;
    
    float mStar = pow(star(uv-vec2(1.3, 0.7), 1.0, smoothstep(0.2, 1.0, 0.45)*.6), 1.0);
    color += mStar*0.8 * vec3(0.702,1.000,0.941);
    
    color += purple * pow(
        wave(vec2(-uvS.x*2.0 + 1.8, (uv.y-0.7) * 1.5)),
        3.0
    );
    
    vec3 sky = pow(
        mix(
            green, purple, 
            saturate(pow(uvS.y-0.03, 0.5) * (uvS.x+0.7) * (1.0-uvS.x+0.5))
        ), vec3(3.0)
    ) * 2.0;
    color += sky;
    
    
    color *= terrain(uv);
    
    uv.x -= 0.5;
    uv.y -= 0.03;
    color *= Humanoid(uv*9.0, 1.0);
    

    // Output to screen
    fragColor = vec4(color, 1.0);
}




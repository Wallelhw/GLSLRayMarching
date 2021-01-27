#define TEXTURED 1

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    vec2 q = fragCoord.xy/iResolution.xy;

#if TEXTURED == 1
    
    vec3 e = vec3(vec2(1.)/iResolution.xy,0.);
    float p10 = texture(iChannel0, q-e.zy).x;
    float p01 = texture(iChannel0, q-e.xz).x;
    float p21 = texture(iChannel0, q+e.xz).x;
    float p12 = texture(iChannel0, q+e.zy).x;
    
    // Totally fake displacement and shading:
    vec3 grad = normalize(vec3(p21 - p01, p12 - p10, 1.));
    vec4 c = texture(iChannel1, fragCoord.xy*2./iChannelResolution[1].xy + grad.xy*.35);
    vec3 light = normalize(vec3(.2,-.5,.7));
    float diffuse = dot(grad,light);
    float spec = pow(max(0.,-reflect(light,grad).z),32.);
    fragColor = mix(c,vec4(.7,.8,1.,1.),.25)*max(diffuse,0.) + spec;
    
#else
    
    float h = texture(iChannel0, q).x;
    float sh = 1.35 - h*2.;
    vec3 c =
       vec3(exp(pow(sh-.75,2.)*-10.),
            exp(pow(sh-.50,2.)*-20.),
            exp(pow(sh-.25,2.)*-10.));
    fragColor = vec4(c,1.);

#endif
}
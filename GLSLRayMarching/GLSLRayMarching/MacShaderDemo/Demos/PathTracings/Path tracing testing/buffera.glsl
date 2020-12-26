void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    vec2 uv = fragCoord/iResolution.xy;
    vec3 bob = texture(iChannel0,vec2(0.5)).xyz;
    vec4 mo = iMouse;
    if (mo == vec4(0)) {
        mo = vec4(0.5);
    }
    vec2 m = bob.xy;
    //if (all(greaterThanEqual(iMouse.zw,vec2(-0.01)))) {
    if (mo.xy != m || all(greaterThan(iMouse.zw,vec2(0)))) {
        fragColor = vec4(mo.xy,1,0);
        return;
    }
    float n = bob.z;
    fragColor = vec4(mo.xy,1.0/(1.0+1.0/n),0);
}
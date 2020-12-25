void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    vec2 uv = fragCoord/iResolution.xy;
    float n = texture(iChannel2,vec2(0.5)).z;//.0/float(iFrame+1);
    //float r = 0.001;
    /*fragColor = texture(iChannel1,(fragCoord+vec2(r,r))/iResolution.xy);
    fragColor += texture(iChannel1,(fragCoord+vec2(-r,r))/iResolution.xy);
    fragColor += texture(iChannel1,(fragCoord+vec2(r,-r))/iResolution.xy);
    fragColor += texture(iChannel1,(fragCoord+vec2(-r,-r))/iResolution.xy);
    fragColor *= (1.0-n)/4.0;*/
    fragColor = texture(iChannel1,uv)*(1.0-n);
    fragColor += sqrt(texture(iChannel0,uv))*n;
    //fragColor = max(texture(iChannel1,uv),sqrt(texture(iChannel0,uv)));
    //fragColor = sqrt(texture(iChannel0,uv));
}
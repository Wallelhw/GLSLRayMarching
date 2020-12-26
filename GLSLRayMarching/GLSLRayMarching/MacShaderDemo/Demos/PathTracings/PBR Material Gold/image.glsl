void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    vec2 uv = fragCoord.xy / iResolution.xy;
	vec3 color = vec3(0.0);

    if( iFrame > 0 ) {
        color = texture( iChannel0, uv ).xyz;
        color /= float( iFrame );
        color = pow( color, vec3(0.4545) );
    }

    fragColor = vec4( color, 1.0 );
}

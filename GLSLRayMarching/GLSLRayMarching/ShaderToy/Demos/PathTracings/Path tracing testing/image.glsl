void mainImage( out vec4 fragColor, in vec2 C )
{
    vec2 R = iResolution.xy;
    fragColor = texture(iChannel0,C/R);
    //fragColor *= log(log(fragColor.w));
}

// Common is a bunch of random stuff
// Buffer A figures out the number of frames that have passed since releasing the mouse
// Buffer B does the raymarching stuff
// Buffer C averages the B buffers
// The image just copies Buffer C
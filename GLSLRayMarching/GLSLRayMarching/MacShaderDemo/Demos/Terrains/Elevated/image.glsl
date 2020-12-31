// Created by inigo quilez - iq/2013
// License Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License.


// on the derivatives based noise: http://iquilezles.org/www/articles/morenoise/morenoise.htm
// on the soft shadow technique: http://iquilezles.org/www/articles/rmshadows/rmshadows.htm
// on the fog calculations: http://iquilezles.org/www/articles/fog/fog.htm
// on the lighting: http://iquilezles.org/www/articles/outdoorslighting/outdoorslighting.htm
// on the raymarching: http://iquilezles.org/www/articles/terrainmarching/terrainmarching.htm


#define AA 1   // make this 2 or even 3 if you have a really powerful GPU

#define GLOBAL_SCALE (250.0)

// value noise, and its analytical derivatives
vec3 noised( in vec2 x )
{
    vec2 f = fract(x);
    vec2 u = f*f*(3.0-2.0*f);

#if 1
    // texel fetch version
    ivec2 p = ivec2(floor(x));
    float a = texelFetch( iChannel0, (p+ivec2(0,0))&255, 0 ).x;
	float b = texelFetch( iChannel0, (p+ivec2(1,0))&255, 0 ).x;
	float c = texelFetch( iChannel0, (p+ivec2(0,1))&255, 0 ).x;
	float d = texelFetch( iChannel0, (p+ivec2(1,1))&255, 0 ).x;
#else    
    // texture version    
    vec2 p = floor(x);
	float a = textureLod( iChannel0, (p+vec2(0.5,0.5))/256.0, 0.0 ).x;
	float b = textureLod( iChannel0, (p+vec2(1.5,0.5))/256.0, 0.0 ).x;
	float c = textureLod( iChannel0, (p+vec2(0.5,1.5))/256.0, 0.0 ).x;
	float d = textureLod( iChannel0, (p+vec2(1.5,1.5))/256.0, 0.0 ).x;
#endif
    
	return vec3(a+(b-a)*u.x+(c-a)*u.y+(a-b-c+d)*u.x*u.y,
				6.0*f*(1.0-f)*(vec2(b-a,c-a)+(a-b-c+d)*u.yx));
}

const mat2 m2 = mat2(0.8,-0.6,0.6,0.8);


float terrainH( in vec2 x )
{
	vec2  p = x*0.003/GLOBAL_SCALE;
    float a = 0.0;
    float b = 1.0;
	vec2  d = vec2(0.0);
    for( int i=0; i<16; i++ )
    {
        vec3 n = noised(p);
        d += n.yz;
        a += b*n.x/(1.0+dot(d,d));
		b *= 0.5;
        p = m2*p*2.0;
    }

	return GLOBAL_SCALE*120.0*a;
}

float terrainM( in vec2 x )
{
	vec2  p = x*0.003/GLOBAL_SCALE;
    float a = 0.0;
    float b = 1.0;
	vec2  d = vec2(0.0);
    for( int i=0; i<9; i++ )
    {
        vec3 n = noised(p);
        d += n.yz;
        a += b*n.x/(1.0+dot(d,d));
		b *= 0.5;
        p = m2*p*2.0;
    }
	return GLOBAL_SCALE*120.0*a;
}

float terrainL( in vec2 x )
{
	vec2  p = x*0.003/GLOBAL_SCALE;
    float a = 0.0;
    float b = 1.0;
	vec2  d = vec2(0.0);
    for( int i=0; i<3; i++ )
    {
        vec3 n = noised(p);
        d += n.yz;
        a += b*n.x/(1.0+dot(d,d));
		b *= 0.5;
        p = m2*p*2.0;
    }

	return GLOBAL_SCALE*120.0*a;
}

float raycast( in vec3 cameraPos, in vec3 rayDir, in float tmin, in float tmax )
{
    float t = tmin;
	for( int i=0; i<300; i++ )
	{
        vec3 pos = cameraPos + t*rayDir;
		float h = pos.y - terrainH( pos.xz );
		if( abs(h)<(0.0015*t) || t>tmax ) break;
		t += 0.4*h;
	}

	return t;
}

float softShadow(in vec3 cameraPos, in vec3 rayDir )
{
    float res = 1.0;
    float t = 0.001;
	for( int i=0; i<80; i++ )
	{
	    vec3  p = cameraPos + t*rayDir;
        float h = p.y - terrainM( p.xz );
		res = min( res, 16.0*h/t );
		t += h;
		if( res<0.001 ||p.y>(GLOBAL_SCALE*200.0) ) break;
	}
	return clamp( res, 0.0, 1.0 );
}

vec3 calcNormal( in vec3 pos, float t )
{
    vec2  eps = vec2( 0.001*t, 0.0 );
    return normalize( vec3( terrainH(pos.xz-eps.xy) - terrainH(pos.xz+eps.xy),
                            2.0*eps.x,
                            terrainH(pos.xz-eps.yx) - terrainH(pos.xz+eps.yx) ) );
}

float fractal_brownian_motion(vec2 p)
{
    float f = 0.0;

    f += 0.5000*texture(iChannel0, p/256.0).x; p = m2*p*2.02;
    f += 0.2500*texture(iChannel0, p/256.0).x; p = m2*p*2.03;
    f += 0.1250*texture(iChannel0, p/256.0).x; p = m2*p*2.01;
    f += 0.0625*texture(iChannel0, p/256.0).x;

    return f/0.9375;
}

float fbm(vec2 p)
{
    return fractal_brownian_motion(p);
}

const float kMaxT = 5000.0*GLOBAL_SCALE;

vec4 render2( in vec3 cameraPos, in vec3 rayDir )
{
    vec3 L = normalize( vec3(-0.8,0.4,-0.3) );

    float tmin = 1.0;
    float tmax = kMaxT;

    float maxh = 300.0*GLOBAL_SCALE;
    float tp = (maxh - cameraPos.y) / rayDir.y;
    if(tp > 0.0)
    {
        if(cameraPos.y > maxh) 
            tmin = max(tmin, tp);
        else            
            tmax = min(tmax, tp);
    }

    float VDotL = clamp(dot(rayDir, L), 0.0, 1.0);

	vec3 col;
    float t = raycast(cameraPos, rayDir, tmin, tmax);
    if(t > tmax)
    {
        // sky color
        #define SKY_BASE_COLOR vec3(0.3,0.5,0.85)
        #define SKY_TINT_COLOR (0.85 * vec3(0.7, 0.75, 0.85))

        col = SKY_BASE_COLOR - rayDir.y * rayDir.y * 0.5;                   // dimmer at height
        col = mix(col, SKY_TINT_COLOR, pow(1.0 - max(rayDir.y,0.0), 4.0));  // blend with SKY_TINE SKY_TINT_COLOR

        // sun
		col += 0.25 * vec3(1.0,0.7,0.4) * pow(VDotL,   5.0);
		col += 0.25 * vec3(1.0,0.8,0.6) * pow(VDotL,  64.0);
		col += 0.20 * vec3(1.0,0.8,0.6) * pow(VDotL, 512.0);

		vec2 sc = cameraPos.xz + rayDir.xz * (GLOBAL_SCALE*1000.0 - cameraPos.y) / rayDir.y;
		col = mix(col, vec3(1.0,0.95,1.0), 0.5 * smoothstep(0.5,0.8, fbm(0.0005 * sc/GLOBAL_SCALE)) );

        col = vec3(1.0,0.95,1.0);

        t = -1.0;
	}
	else
	{
        // mountain color
        col = vec3(1.0, 0.0, 0.0);
	}
    

    // gamma correction
	col = sqrt(col);
    
	return vec4( col, t );
}

vec4 render( in vec3 cameraPos, in vec3 rayDir )
{
    vec3 light1 = normalize( vec3(-0.8,0.4,-0.3) );
    // bounding plane
    float tmin = 1.0;
    float tmax = kMaxT;

#if 1
    float maxh = 300.0*GLOBAL_SCALE;
    float tp = (maxh-cameraPos.y)/rayDir.y;
    if( tp>0.0 )
    {
        if( cameraPos.y>maxh ) 
            tmin = max( tmin, tp );
        else            
            tmax = min( tmax, tp );
    }
#endif

	float sundot = clamp(dot(rayDir, light1), 0.0, 1.0);
	vec3 col;
    float t = raycast( cameraPos, rayDir, tmin, tmax );
    if(t > tmax)
    {
        // sky		
        col = vec3(0.3,0.5,0.85) - rayDir.y * rayDir.y * 0.5;
        col = mix(col, 0.85 * vec3(0.7, 0.75, 0.85), pow(1.0 - max(rayDir.y,0.0), 4.0));

        // sun
		col += 0.25*vec3(1.0,0.7,0.4)*pow( sundot,5.0 );
		col += 0.25*vec3(1.0,0.8,0.6)*pow( sundot,64.0 );
		col += 0.2*vec3(1.0,0.8,0.6)*pow( sundot,512.0 );

        // clouds
		vec2 sc = cameraPos.xz + rayDir.xz*(GLOBAL_SCALE*1000.0-cameraPos.y)/rayDir.y;
		col = mix( col, vec3(1.0,0.95,1.0), 0.5*smoothstep(0.5,0.8, fbm(0.0005*sc/GLOBAL_SCALE)) );

        // horizon
        col = mix( col, 0.68*vec3(0.4,0.65,1.0), pow( 1.0-max(rayDir.y,0.0), 16.0 ) );

        t = -1.0;
	}
	else
	{
        // mountains		
		vec3 P = cameraPos + t * rayDir;
        vec3 N = calcNormal( P, t );
        vec3 R = reflect( rayDir, N );
        float F = clamp(1.0 + dot(rayDir, N), 0.0, 1.0 );
        vec3 H = normalize(light1-rayDir);

        // rock
		float r = texture( iChannel0, (7.0/GLOBAL_SCALE)*P.xz/256.0 ).x;
        col = (r*0.25+0.75)*0.9*mix( vec3(0.08,0.05,0.03), vec3(0.10,0.09,0.08), 
                                     texture(iChannel0,0.00007*vec2(P.x,P.y*48.0)/GLOBAL_SCALE).x );
		col = mix( col, 0.20*vec3(0.45,.30,0.15)*(0.50+0.50*r),smoothstep(0.70,0.9,N.y) );
        
        
        col = mix( col, 0.15*vec3(0.30,.30,0.10)*(0.25+0.75*r),smoothstep(0.95,1.0,N.y) );
		col *= 0.1+1.8*sqrt(fbm(P.xz*0.04)*fbm(P.xz*0.005));
        
		// snow
		float h = smoothstep(55.0, 80.0, P.y/GLOBAL_SCALE + 25.0*fbm(0.01*P.xz/GLOBAL_SCALE) );
        float e = smoothstep(1.0-0.5*h,1.0-0.1*h,N.y);
        float o = 0.3 + 0.7*smoothstep(0.0,0.1,N.x+h*h);
        float s = h*e*o;
        col = mix( col, 0.29*vec3(0.62,0.65,0.7), smoothstep( 0.1, 0.9, s ) );

        // lighting		
        float amb = clamp(0.5+0.5*N.y,0.0,1.0);
		float dif = clamp( dot( light1, N ), 0.0, 1.0 );
		float bac = clamp( 0.2 + 0.8*dot( normalize( vec3(-light1.x, 0.0, light1.z ) ), N ), 0.0, 1.0 );
		float sh = 1.0; if( dif>=0.0001 ) sh = softShadow(P+light1*GLOBAL_SCALE*0.05,light1);

		vec3 lin  = vec3(0.0);
		lin += dif*vec3(8.00,5.00,3.00)*1.3*vec3( sh, sh*sh*0.5+0.5*sh, sh*sh*0.8+0.2*sh );
		lin += amb*vec3(0.40,0.60,1.00)*1.2;
        lin += bac*vec3(0.40,0.50,0.60);
		col *= lin;
        
        col += (0.7+0.3*s)*(0.04+0.96*pow(clamp(1.0+dot(H,rayDir),0.0,1.0),5.0))*
               vec3(7.0,5.0,3.0)*dif*sh*
               pow( clamp(dot(N,H), 0.0, 1.0),16.0);
        
        col += s*0.65*pow(F,4.0)*vec3(0.3,0.5,0.6)*smoothstep(0.0,0.6,R.y);

		// fog
        float fo = 1.0-exp(-pow(0.001*t/GLOBAL_SCALE,1.5) );
        vec3 fco = 0.65*vec3(0.4,0.65,1.0);
        col = mix( col, fco, fo );
	}
    
    // sun
    //col += 0.3*vec3(1.0,0.7,0.3)*pow( sundot, 8.0 );

    // gamma correction
	col = sqrt(col);
    
	return vec4( col, t );
}

mat3 setCamera( in vec3 cameraPos, in vec3 cameraTarget, in float cameraRoll )
{
	vec3 cameraZAxis = normalize(cameraTarget - cameraPos);
	vec3 cameraUp = vec3(sin(cameraRoll), cos(cameraRoll), 0.0);
	vec3 cameraXAxis = normalize(cross(cameraZAxis, cameraUp) );
	vec3 cameraYAxis = normalize(cross(cameraXAxis, cameraZAxis) );
    return mat3( cameraXAxis, cameraYAxis, cameraZAxis );
}

vec3 camPath( float time )
{
	return GLOBAL_SCALE*1100.0*vec3( cos(0.0+0.23*time), 0.0, cos(1.5+0.21*time) );
}

void moveCamera( float time, out vec3 oCameraPos, out vec3 oCameraTarget, out float oCameraRoll, out float oFlimDistance )
{
	oCameraPos = camPath( time );
	oCameraTarget = camPath( time + 3.0 );
	oCameraPos.y = terrainL( oCameraPos.xz ) + 22.0*GLOBAL_SCALE;
	oCameraTarget.y = oCameraPos.y - 20.0*GLOBAL_SCALE;

    oCameraRoll = 0.2*cos(0.1*time);
    oFlimDistance = 3.0;
}

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    float time = iTime*0.1 - 0.1 + 0.3 + 4.0*iMouse.x/iResolution.x;
    time = - 0.1 + 0.3 + 4.0*iMouse.x/iResolution.x;
    // camera position
    vec3 cameraPos, cameraTarget; 
    float cameraRoll, flimDistance;
    moveCamera(time, cameraPos, cameraTarget, cameraRoll, flimDistance);

    // camera2world transform    
    mat3 cam = setCamera(cameraPos, cameraTarget, cameraRoll);

    // pixel
    vec2 p = (-iResolution.xy + 2.0*fragCoord) / iResolution.y;

    float t = kMaxT;
	#if AA>1
        vec3 tot = vec3(0.0);

        for( int m=0; m<AA; m++ )
        {
            for( int n=0; n<AA; n++ )
            {
                // pixel coordinates
                vec2 offset = vec2(float(m), float(n)) / float(AA) - 0.5;
                vec2 samplePosition = (-iResolution.xy + 2.0 * (fragCoord + offset)) / iResolution.y;
        
                // camera ray    
                vec3 rayDir = cam * normalize(vec3(samplePosition, flimDistance));
        
                vec4 res = render(cameraPos, rayDir);
                t = min(t, res.w);
         
                tot += res.xyz;
            }
        }
        tot /= float(AA*AA);
    #else
        vec3 tot = vec3(0.0);

        vec2 s = p;

        vec3 rayDir = cam * normalize(vec3(s, flimDistance));

        vec4 res = render(cameraPos, rayDir);
        t = min( t, res.w );
 
        tot += res.xyz;
	#endif



    float vel = 0.0;
    if( t<0.0 )
    {
        vel = -1.0;
    }
    else
    {
        // old camera position
        float oldTime = time - 0.1 * 1.0/24.0; // 1/24 of a second blur
        vec3 oldRo, oldTa; float oldCr, oldFl;
        moveCamera( oldTime, oldRo, oldTa, oldCr, oldFl );
        mat3 oldCam = setCamera( oldRo, oldTa, oldCr );

        // world space
        #if AA>1
        vec3 rayDir = cam * normalize(vec3(p,fl));
        #endif
        vec3 wpos = cameraPos + rayDir * t;
        // camera space
        vec3 cpos = vec3( dot( wpos - oldRo, oldCam[0] ),
                          dot( wpos - oldRo, oldCam[1] ),
                          dot( wpos - oldRo, oldCam[2] ) );
        // ndc space
        vec2 npos = oldFl * cpos.xy / cpos.z;
        // screen space
        vec2 spos = 0.5 + 0.5*npos*vec2(iResolution.y/iResolution.x,1.0);

        // compress velocity vector in a single float
        vec2 uv = fragCoord/iResolution.xy;
        spos = clamp( 0.5 + 0.5*(spos - uv)/0.25, 0.0, 1.0 );
        vel = floor(spos.x*1023.0) + floor(spos.y*1023.0)*1024.0;
    }
    
    fragColor = vec4( tot, vel );
}
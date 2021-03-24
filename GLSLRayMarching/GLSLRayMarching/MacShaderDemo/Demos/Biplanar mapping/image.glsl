// The MIT License
// Copyright © 2020 Inigo Quilez
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software. THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


// An alternative to box mapping (a.k.a. "roundcube" or 
// "triplanar" mapping), where some extra ALU computations
// are performed but one texture fetch is avoided, for a
// total of just two.

// The idea is that instead of doing the 3 planar projections
// (in the X, Y and Z directions) and later blend them
// together based on the alignment of the normal vector to
// each of those three directions, we can perhaps get away
// with picking only the two most relevant projection
// directions out of the three and ignore the third.
// That introduces in theory some discontinutity but it
// seems is not noticeable, and it saves a precious
// texture fetch.

// Note that the texture coord derivatives need to be taken
// before the projection axis selections are done, to
// prevent filtering issue. Check the biplanar() function
// below.
//
// More information here:
//
// https://iquilezles.org/www/articles/biplanar/biplanar.htm
//
// For a more complicated example of biplanar texturing:
//
// https://www.shadertoy.com/view/3ddfDj
//
// Also for comparison, here's traditional boxmapping:
//
// https://www.shadertoy.com/view/MtsGWH


vec4 biplanar( sampler2D sam, in vec3 p, in vec3 n, in float k )
{
    // grab coord derivatives for texturing
    vec3 dpdx = dFdx(p);
    vec3 dpdy = dFdy(p);
    n = abs(n);

    // major axis (in x; yz are following axis)
    ivec3 ma = (n.x>n.y && n.x>n.z) ? ivec3(0,1,2) :
               (n.y>n.z)            ? ivec3(1,2,0) :
                                      ivec3(2,0,1) ;
    // minor axis (in x; yz are following axis)
    ivec3 mi = (n.x<n.y && n.x<n.z) ? ivec3(0,1,2) :
               (n.y<n.z)            ? ivec3(1,2,0) :
                                      ivec3(2,0,1) ;
        
    // median axis (in x;  yz are following axis)
    ivec3 me = ivec3(3) - mi - ma;
    
    // project+fetch
    vec4 x = textureGrad( sam, vec2(   p[ma.y],   p[ma.z]), 
                               vec2(dpdx[ma.y],dpdx[ma.z]), 
                               vec2(dpdy[ma.y],dpdy[ma.z]) );
    vec4 y = textureGrad( sam, vec2(   p[me.y],   p[me.z]), 
                               vec2(dpdx[me.y],dpdx[me.z]),
                               vec2(dpdy[me.y],dpdy[me.z]) );
    
    // blend and return
    vec2 m = vec2(n[ma.x],n[me.x]);
    // optional - add local support (prevents discontinuty)
    m = clamp( (m-0.5773)/(1.0-0.5773), 0.0, 1.0 );
    // transition control
    m = pow( m, vec2(k/8.0) );
	return (x*m.x + y*m.y) / (m.x + m.y);
}


//===============================================================================================

#if HW_PERFORMANCE==0
#define AA 1
#else
#define AA 2
#endif

float smin( float a, float b, float k )
{
	float h = clamp( 0.5 + 0.5*(b-a)/k, 0.0, 1.0 );
	return mix( b, a, h ) - k*h*(1.0-h);
}

float map( in vec3 p )
{
	float d = length(p-vec3(0.0,1.0,0.0))-1.0;
    d = smin( d, p.y, 1.0 );
    return d;
}

// http://iquilezles.org/www/articles/normalsSDF/normalsSDF.htm
vec3 calcNormal( in vec3 pos, in float eps )
{
    vec2 e = vec2(1.0,-1.0)*0.5773*eps;
    return normalize( e.xyy*map( pos + e.xyy ) + 
					  e.yyx*map( pos + e.yyx ) + 
					  e.yxy*map( pos + e.yxy ) + 
					  e.xxx*map( pos + e.xxx ) );
}

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    vec3 tot = vec3(0.0);
#if AA>1
    for( int m=0; m<AA; m++ )
    for( int n=0; n<AA; n++ )
    {
        // pixel coordinates
        vec2 o = vec2(float(m),float(n)) / float(AA) - 0.5;
        vec2 p = (2.0*(fragCoord+o)-iResolution.xy)/iResolution.y;
#else    
        vec2 p = (2.0*fragCoord-iResolution.xy)/iResolution.y;
#endif

        // camera movement	
        float an = 0.2*iTime;
        vec3 ro = vec3( 2.5*sin(an), 2.0, 2.5*cos(an) );
        vec3 ta = vec3( 0.0, 1.0, 0.0 );
        // camera matrix
        vec3 ww = normalize( ta - ro );
        vec3 uu = normalize( cross(ww,vec3(0.0,1.0,0.0) ) );
        vec3 vv = normalize( cross(uu,ww));
        // create view ray
        vec3 rd = normalize( p.x*uu + p.y*vv + 1.5*ww );

        // raymarch
        const float tmax = 8.0;
        float t = 1.0;
        for( int i=0; i<128; i++ )
        {
            vec3 pos = ro + rd*t;
            float h = map(pos);
            if( h<0.001 ) break;
            t += h;
            if( t>tmax ) break;
        }
        
        vec3 col = vec3(0.0);
        if( t<tmax )
        {
            vec3 pos = ro + rd*t;
            vec3 nor = calcNormal( pos, 0.001 );
            float occ = clamp(0.4 + 0.6*nor.y, 0.0, 1.0);
            col = biplanar( iChannel0, 0.5*pos, nor, 8.0 ).xyz;
            col = col*col;
            col *= occ;
            col *= 2.0;
            col *= 1.0-smoothstep(1.0,6.0,length(pos.xz));
        }
        // to gamma space
        col = sqrt( col );
        tot += col;
#if AA>1
    }
    tot /= float(AA*AA);
#endif

	fragColor = vec4( tot, 1.0 );
}
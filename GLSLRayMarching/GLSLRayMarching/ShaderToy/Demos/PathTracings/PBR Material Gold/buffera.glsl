// Except where otherwise noted:

// Copyright ? 2018 Markus Moenig Distributed under The MIT License.
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software. THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//

// 0 - Wall, 1 - Sphere, 2 - Cylinder, 3 - Twisted, 4 - Floor, 5 - Preview
#define SHAPE 5

// Displacement / Bump Mapping switch
#define DISPLACEMENT 1

// Path-tracing switch and depth
#define PATH 1
#define PATH_DEPTH 2

// --- Material Code

struct Material {
    int type;
    vec3 color;
    vec3 specularColor;
    float specularAmount;
    float metallic;
    float smoothness;
    float reflectance;
    float bump;
    vec3 emission;
    float ior;
};

// --- Exported by Material-Z.com. Currently in early alpha.

vec3 source0( in vec3 inPos, in vec2 inUV, in vec3 inNormal, in float inTime, in vec3 inInput1, in vec3 inInput2, in vec3 inInput3 ) {
  float noise = fract(sin(dot(inUV.xy, vec2(12.9898,78.233))) * 43758.5453123);
inInput1 = vec3( noise );
return inInput1;
}

vec3 source1( in vec3 inPos, in vec2 inUV, in vec3 inNormal, in float inTime, in vec3 inInput1, in vec3 inInput2, in vec3 inInput3 ) {
  // Based on IQs value noise https://www.shadertoy.com/view/lsf3WH.
  vec2 x = inUV / 0.374 * vec2( 1.000, 1.000 );
  vec2 p = floor(x);
  vec2 f = fract(x);
  f = f*f*(3.0-2.0*f);
  float noise = mix(mix( source0(inPos, p, inNormal, inTime, inInput1, inInput2, inInput3 ).x,
                          source0(inPos, p + vec2( 1.0, 0.0), inNormal, inTime, inInput1, inInput2, inInput3 ).x,f.x),
                     mix( source0(inPos, p + vec2( 0.0, 1.0), inNormal, inTime, inInput1, inInput2, inInput3 ).x,
                          source0(inPos, p + vec2( 1.0, 1.0), inNormal, inTime, inInput1, inInput2, inInput3 ).x, f.x),f.y);
inInput1 = vec3( noise );
return inInput1;
}

vec3 source2( in vec3 inPos, in vec2 inUV, in vec3 inNormal, in float inTime, in vec3 inInput1, in vec3 inInput2, in vec3 inInput3 ) {
  float value = 0.0;
  float amplitude = 1.;
  vec2 p = inUV;
  for (int i = 0; i < 16; i++)
  {
      value += amplitude * source1( inPos, p, inNormal, inTime, inInput1, inInput2, inInput3 ).x;
      p *= 3.141;
      amplitude *= 0.668;
  }
inInput1 = vec3( value );
return inInput1;
}


void material0( in vec3 pos, inout vec3 normal, in float time, inout Material material ) {
    vec2 uv;
    material.color = vec3( 1, 0.7098039215686275, 0.2901960784313726 );material.specularColor = vec3( 1, 1, 1 );material.specularAmount = 0.000;material.smoothness = 0.820;material.reflectance = 1.000;
    vec3 n = abs( normal );
    if( n.x > 0.57735 ) uv = pos.yz;
    else if (n.y>0.57735) uv = pos.xz;
    else uv = pos.xy;
    material.bump = 0.5;
    vec3 _oLOw = source0( pos, uv, normal, 0., vec3(0), vec3(0), vec3(0) );
    vec3 _YWZs = source1( pos, uv, normal, 0., _oLOw, vec3(0), vec3(0) );
    vec3 _cvoZ = source2( pos, uv, normal, 0., _YWZs, vec3(0), vec3(0) );
    vec3 _lWDq = 1.000 - _cvoZ;
    vec3 _WzWp = _lWDq * 0.100 * vec3( 1.000, 1.000, 1.000 );
    material.metallic = _cvoZ.x;
    material.bump = _WzWp.x;
}

// ---

struct Light {
    float id;
    vec3 emission;
};

struct Ray {
    vec3 origin;
    vec3 dir;
};

// --- IQs distance functions
// --- http://www.iquilezles.org/www/articles/distfunctions/distfunctions.htm

float smin( float a, float b, float k )
{
    float res = exp( -k*a ) + exp( -k*b );
    return -log( res )/k;
}
vec3 opBlend( vec3 d1, vec3 d2, float k )
{
    vec3 rc;
    rc.x=smin( d1.x, d2.x, k );

    if ( d1.x < d2.x )
    {
        rc.y=d1.y;
        rc.z=d1.z;
    } else
    {
        rc.y=d2.y;
        rc.z=d2.z;
    }

    return rc;
}

vec3 opU( vec3 d1, vec3 d2 )
{
    return (d1.x<d2.x) ? d1 : d2;
}

vec3 opTwist( vec3 p, float twist )
{
    float c = cos(twist*p.z);
    float s = sin(twist*p.z);
    mat2  m = mat2(c,-s,s,c);
    vec3  q = vec3(m*p.xy,p.z);
    return q;
}

float opS( float d1, float d2 )
{
    return max(-d2,d1);
}

float sdCylinder( vec3 p, vec2 h )
{
    vec2 d = abs(vec2(length(p.xz),p.y)) - h;
    return min(max(d.x,d.y),0.0) + length(max(d,0.0));
}

// ---

#define PI 3.1415926535897932384626422832795028841971

#define LIGHT1_POS vec3( 0.5, 0.5, 8 )
#define LIGHT1_EM vec3( 180*5, 180 *5, 180 *5 )

#define SUN_POS vec3( 0.5, 3.5, 3 )
#define SUN_EM vec3( 180*5, 180 *5, 180 *5 )

vec2 randv2;

float random() {
    randv2+=vec2(1.0,0.0);
    // return fract(sin(dot(vTexCoord, vec2(12.9898, 78.233)) + seed++) * 43758.5453);
    return fract(sin(dot(randv2.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

vec2 rand2(){// implementation derived from one found at: lumina.sourceforge.net/Tutorials/Noise.html
    randv2+=vec2(1.0,1.0);
    return vec2(fract(sin(dot(randv2.xy ,vec2(12.9898,78.233))) * 43758.5453),
        fract(cos(dot(randv2.xy ,vec2(4.898,7.23))) * 23421.631));
}

// --- Map functions

#if (SHAPE != 5)

vec3 map( vec3 p )
{
    vec3 res=vec3( 1000000, -2, -1 );

    #if (SHAPE == 0)
    res=opU( res, vec3( p.z, 0, 0 ) );
    #elif (SHAPE == 1)
    res = opU( res, vec3( length( p ) - 1.0, 0, 0 ) );
    #elif (SHAPE == 2)
    vec2 h = vec2( 0.95, 0.8 ); vec2 d = abs(vec2(length(p.xz),p.y)) - h;
    res=opU( res, vec3( min(max(d.x,d.y),0.0) + length(max(d,0.0)), 0, 0 ) );
    #elif (SHAPE == 3)
    vec3 tp=p;tp.zxy=opTwist( tp.zxy, 2.531 );
    res=opU( res, vec3( length( max( abs(tp) - vec3( 0.213 * 3.0, 0.978 * 3.0, 0.213 * 3.0 ), 0.0 ) ) - 0.10, 0, 0 ) );
    #elif (SHAPE == 4)
    res=opU( res, vec3( p.y + 1.5, 0, 0 ) );
    #endif

#if (DISPLACEMENT == 1)
    vec3 normal; Material material;
    material0( p, normal, 0., material );
    res.x -=material.bump/50.;
#endif
    res = opU( res, vec3( length( p - LIGHT1_POS ) - 1.0, 2, 2 ) );

    return res;
}

#elif (SHAPE == 5)

vec3 map( vec3 p )
{
    // --- Exported by RaySupreme.com
    vec3 res=vec3( 1000000, -2, -1 ); mat4 mat; vec3 tp, temp;
    vec3 gResult1, gResult2;
    gResult1 = vec3( 1000000, -2, -1 );
    tp=p;
    gResult1=opU( gResult1, vec3( length( max( abs( tp) - vec3( 399.741, 399.741, 399.741 ), 0.0 ) ) - 0.259, 0, 0 ) );
    tp=p;
    tp.y = tp.y + -19.0000;
    gResult1.x=opS( gResult1.x, length( max( abs( tp) - vec3( 84.000, 3.000, 3.000 ), 0.0 ) ) - 16.000);
    res=opU( res, gResult1 );
    tp=p;
    tp = tp + vec3(-2.0000,-2.0000,-4.0000);
    res=opU( res, vec3( length( tp ) - 1.000, 1, 3 ) );
    tp=p;
    tp = tp + vec3(4.0000,-1.5000,-4.0000);
    res=opU( res, vec3( length( tp ) - 0.300, 1, 4 ) );
    gResult1 = vec3( 1000000, -2, -1 );
    gResult2 = vec3( 1000000, -2, -1 );
    tp=p;
    tp.x = tp.x + -0.3542;
    tp /= vec3( 1.300, 1.300, 1.300 );
    gResult2=opU( gResult2, vec3( sdCylinder( tp, vec2( 1.000, 0.030) ), 3, 5 ) );
    tp=p;
    tp.xy = tp.xy + vec2(-0.3542,-0.1300);
    tp /= vec3( 1.300, 1.300, 1.300 );
#if (DISPLACEMENT == 1)
    vec3 bumpNormal; Material bumpMaterial;
    material0( p, bumpNormal, 0., bumpMaterial );
    gResult2 = opBlend( gResult2, vec3( sdCylinder( tp, vec2( 0.800, 0.030) )- bumpMaterial.bump/50., 2, 6 ), 16.7396 );
 #else
    gResult2 = opBlend( gResult2, vec3( sdCylinder( tp, vec2( 0.800, 0.030) ), 2, 6 ), 16.7396 );
 #endif
    tp=p;
    tp.xy = tp.xy + vec2(-0.3542,-0.1560);
    tp /= vec3( 1.300, 1.300, 1.300 );
    gResult2.x=opS( gResult2.x, sdCylinder( tp, vec2( 0.700, 0.040) ));
    gResult1=opU( gResult1, gResult2 );
    gResult2 = vec3( 1000000, -2, -1 );
    tp=p;
    mat=mat4(0.3323,-0.1160,-0.9360,0.0000
    ,0.3295,0.9442,0.0000,0.0000
    ,0.8838,-0.3084,0.3519,0.0000
    ,-0.4378,-0.8764,0.3315,1.0000);
    tp=(mat * vec4(tp, 1.0)).xyz;
    tp /= vec3( 1.495, 1.495, 1.495 );
#if (DISPLACEMENT == 1)
    material0( p, bumpNormal, 0., bumpMaterial );
    gResult2=opU( gResult2, vec3( length( tp ) - 0.600- bumpMaterial.bump/50., 2, 9 ) );
#else
    gResult2=opU( gResult2, vec3( length( tp ) - 0.600, 2, 9 ) );
#endif
    tp=p;
    tp.xz = mat2(0.8400,-0.5426,0.5426,0.8400) * tp.xz;
    tp = tp + vec3(-0.3472,-1.4950,-2.3441);
    tp /= vec3( 1.495, 1.495, 1.495 );
    gResult2.x=opS( gResult2.x, length( tp ) - 1.280);
    tp=p;
    mat=mat4(0.8400,0.0942,-0.5344,0.0000
    ,-0.0000,0.9848,0.1736,0.0000
    ,0.5426,-0.1459,0.8272,0.0000
    ,-0.2975,-0.9904,0.0205,1.0000);
    tp=(mat * vec4(tp, 1.0)).xyz;
    tp /= vec3( 1.495, 1.495, 1.495 );
    gResult2.x=opS( gResult2.x, sdCylinder( tp, vec2( 0.630, 0.030) ));
    tp=p;
    tp.xz = mat2(0.8400,-0.5426,0.5426,0.8400) * tp.xz;
    tp = tp + vec3(-0.2975,-0.9717,0.1922);
    tp /= vec3( 1.495, 1.495, 1.495 );
    gResult2.x=opS( gResult2.x, length( tp ) - 0.550);
    tp=p;
    tp.xz = mat2(0.8400,-0.5426,0.5426,0.8400) * tp.xz;
    tp = tp + vec3(-0.2975,-0.9717,0.1922);
    tp /= vec3( 1.495, 1.495, 1.495 );
    gResult2=opU( gResult2, vec3( length( tp ) - 0.520, 0, 13 ) );
    gResult1=opU( gResult1, gResult2 );
    res=opU( res, gResult1 );
    return res;
}

#endif

// --- Normal

vec3 calcNormal( in vec3 pos )
{
    vec2 e = vec2(1.0,-1.0)*0.5773*0.0005;
    return normalize( e.xyy*map( pos + e.xyy ).x +
                    e.yyx*map( pos + e.yyx ).x +
                    e.yxy*map( pos + e.yxy ).x +
                    e.xxx*map( pos + e.xxx ).x );
}

// --- IQs raycasting

vec3 castRay( in vec3 ro, in vec3 rd, in float tmin, in float tmax )
{
    float t=tmin;
    float m=-1.0, id=-1.0;

    for( int i=0; i<100; i++ )
    {
        float precis = 0.0005*t;

        vec3 res = map( ro+rd*t );
        if( t<precis || t>tmax ) break;
        t += res.x;
        m = res.y;
        id = res.z;
    }

    if( t>tmax ) { m=-1.0; id=-1.0; }
    return vec3( t, m, id );
}

// --- PBR / GGX

float ggx(vec3 N, vec3 V, vec3 L, float roughness, float F0)
{
    vec3 H = normalize(V + L);

    float dotLH = max(dot(L, H), 0.0);
    float dotNH = max(dot(N, H), 0.0);
    float dotNL = max(dot(N, L), 0.0);
    float dotNV = max(dot(N, V), 0.0);

    float alpha = roughness * roughness + 0.0001;

    // GGX normal distribution
    float alphaSqr = alpha * alpha;
    float denom = dotNH * dotNH * (alphaSqr - 1.0) + 1.0;
    float D = alphaSqr / (denom * denom);

    // Fresnel term approximation
    float F_a = 1.0;
    float F_b = pow(1.0 - dotLH, 5.0);
    float F = mix(F_b, F_a, F0);

    // GGX self shadowing term
    float k = (alpha + 2.0 * roughness + 1.0) / 8.0;
    float G = dotNL / (mix(dotNL, 1.0, k) * mix(dotNV, 1.0, k));

    // '* dotNV' - Is canceled due to normalization
    // '/ dotLN' - Is canceled due to lambert
    // '/ dotNV' - Is canceled due to G
    return max(0.0, min(10.0, D * F * G / 4.0));
}

#if (PATH == 0)
// --- If no path tracing, use IQs softshadow to add detail
// --- http://www.iquilezles.org/www/articles/rmshadows/rmshadows.htm

float softshadow( in vec3 ro, in vec3 rd )
{
    float res = 1.0;
    for( float t=0.02; t < 2.; )
    {
        float h = map(ro + rd*t).x;
        if( h<0.001 ) return 0.0;
        res = min( res, 8.0*h/t );
        t += h;
    }
    return res;
}
#endif

// --- Light Sampling

vec3 angleToDir(vec3 n, float theta, float phi)
{
    float sinPhi = sin(phi);
    float cosPhi = cos(phi);
    vec3 w = normalize(n);
    vec3 u = normalize(cross(w.yzx, w));
    vec3 v = cross(w, u);
    return (u * cos(theta) + v * sin(theta)) * sinPhi + w * cosPhi;
}

vec3 jitter(vec3 d, float phi, float sina, float cosa) {
    vec3 w = normalize(d), u = normalize(cross(w.yzx, w)), v = cross(w, u);
    return (u*cos(phi) + v*sin(phi)) * sina + w * cosa;
}

vec3 sampleLightBRDF( in vec3 hitOrigin, in vec3 hitNormal, in vec3 rayDir, in Material material  )
{
    vec3 brdf = vec3( 0 );
    vec3 s = vec3( 0 );

    Light light;
    light.id = 3.0;
    light.emission = LIGHT1_EM;

    vec3 l0 = vec3( 2, 2, 4 ) - hitOrigin;

    float cos_a_max = sqrt(1. - clamp(0.5 * 0.5 / dot(l0, l0), 0., 1.));
    float cosa = mix(cos_a_max, 1., random());
    vec3 l = jitter(l0, 2.*PI*random(), sqrt(1. - cosa*cosa), cosa);

#if (PATH == 1)
    vec3 lightHit = castRay( hitOrigin, l, 0.001, 100.0 );
    if ( lightHit.z == light.id )
#else
    s += softshadow( hitOrigin, normalize(l0) );
#endif
    {
        float roughness = 1.0 - material.smoothness * material.smoothness;
        float metallic = material.metallic;

        float omega = 2. * PI * (1. - cos_a_max);
        brdf += ((light.emission * clamp(ggx( hitNormal, rayDir, l, roughness, metallic),0.,1.) * omega) / PI);
    }

    light.id = 4.0;

    l0 = vec3( -4, 1.5, 4 ) - hitOrigin;

    cos_a_max = sqrt(1. - clamp(0.5 * 0.5 / dot(l0, l0), 0., 1.));
    cosa = mix(cos_a_max, 1., random());
    l = jitter(l0, 2.*PI*random(), sqrt(1. - cosa*cosa), cosa);

#if (PATH == 1)
    lightHit = castRay( hitOrigin, l, 0.001, 100.0 );
    if ( lightHit.z == light.id )
#else
    s += softshadow( hitOrigin, normalize(l0) );
#endif
    {
        float roughness = 1.0 - material.smoothness * material.smoothness;
        float metallic = material.metallic;

        float omega = 2. * PI * (1. - cos_a_max);
        brdf += ((light.emission * clamp(ggx( hitNormal, rayDir, l, roughness, metallic),0.,1.) * omega) / PI);
    }

#if (PATH == 0)
    brdf *= clamp( s, 0., 1. );
#endif

    return brdf;
}

vec3 sampleLightE( in vec3 hitOrigin, in vec3 hitNormal, in vec3 rayDir, in Material material  )
{
    vec3 e = vec3( 0 );
    vec3 s = vec3( 0 );

    Light light;
    light.id = 3.0;
    light.emission = LIGHT1_EM;

    vec3 l0 = LIGHT1_POS - hitOrigin;

    float cos_a_max = sqrt(1. - clamp(0.5 * 0.5 / dot(l0, l0), 0., 1.));
    float cosa = mix(cos_a_max, 1., random());
    vec3 l = jitter(l0, 2.*PI*random(), sqrt(1. - cosa*cosa), cosa);

#if (PATH == 1)
    vec3 lightHit = castRay( hitOrigin, l, 0.001, 100.0 );
    if ( lightHit.z == light.id )
#else
    s += softshadow( hitOrigin, normalize(l0) );
#endif
    {
        float omega = 2. * PI * (1. - cos_a_max);
        vec3 n = normalize(hitOrigin - LIGHT1_POS);
        e += ((light.emission * clamp(dot(l, n),0.,1.) * omega) / PI);
    }

    light.id = 4.0;

    l0 = vec3( -4, 1.5, 4 ) - hitOrigin;

    cos_a_max = sqrt(1. - clamp(0.5 * 0.5 / dot(l0, l0), 0., 1.));
    cosa = mix(cos_a_max, 1., random());
    l = jitter(l0, 2.*PI*random(), sqrt(1. - cosa*cosa), cosa);

#if (PATH == 1)
    lightHit = castRay( hitOrigin, l, 0.001, 100.0 );
    if ( lightHit.z == light.id )
#else
    s += softshadow( hitOrigin, normalize(l0) );
#endif
    {
        float omega = 2. * PI * (1. - cos_a_max);
        vec3 n = normalize(hitOrigin - vec3( -4, 1.5, 4 ));
        e += ((light.emission * clamp(dot(l, n),0.,1.) * omega) / PI);
    }

#if (PATH == 0)
    e *= clamp( s, 0., 1. );
#endif

    return e;
}

#if (DISPLACEMENT == 0)

// --- IQs screen space bump mapping: https://www.shadertoy.com/view/ldSGzR

vec3 doBump( in vec3 pos, in vec3 nor, in float signal, in float scale )
{
    // build frame
    vec3  s = dFdx( pos );
    vec3  t = dFdy( pos );
    vec3  u = cross( t, nor );
    vec3  v = cross( nor, s );
    float d = dot( s, u );

    // compute bump
    float bs = dFdx( signal );
    float bt = dFdy( signal );

    // offset normal
#if 1
	return normalize( nor - scale*(bs*u + bt*v)/d );
#else
    // if you cannot ensure the frame is not null
	vec3 vSurfGrad = sign( d ) * ( bs * u + bt * v );
    return normalize( abs(d)*nor - scale*vSurfGrad );
#endif
}
#endif

// --- Calculate the color for the given ray

vec4 getColor( in Ray ray )
{
    vec4 tcol = vec4(0,0,0,0);
    vec4 fcol = vec4(1.0);

#if (PATH == 1)
    for (int depth = 0; depth < PATH_DEPTH; depth++)
#endif
    {
        Material material;
        vec3 normal;

        vec3 hit = castRay( ray.origin, ray.dir, 0.001, 100.0 );

        if ( hit.y >= 0.0 )
        {
            vec3 hitOrigin = ray.origin + ray.dir * hit.x;
            normal = calcNormal( hitOrigin );

#if (SHAPE != 5)
            material0( hitOrigin, normal, 0., material );
#if (DISPLACEMENT == 0)
            // --- Bump Mapping
		    normal = doBump( hitOrigin, normal, dot(vec3(material.bump),vec3(0.33)), 0.02 );
#endif
#else
            // --- Hardcoded environment materials for the preview shape
	        material.emission = vec3( 0 );
    	    material.specularColor = vec3( 1 );
        	material.specularAmount = 0.;

            if ( hit.y == 0.0 ) {
            	material.type = 0;
            	material.color = vec3( 0.996, 0.929, 0.929 );
            	material.metallic = 0.;
            	material.smoothness = 0.;
            	material.reflectance = 1.;
        	} else
        	if ( hit.y == 1.0 ) {
            	material.type = 2;
            	material.emission = vec3( 200 );
        	} else
        	if ( hit.y == 2.0 ) {
            	material0( hitOrigin, normal, 0., material );
#if (DISPLACEMENT == 0)
            	// --- Bump Mapping
		    	normal = doBump( hitOrigin, normal, dot(vec3(material.bump),vec3(0.33)), 0.02 );
#endif
            } else
            if ( hit.y == 3.0 ) {
                material.type = 0;
                material.color = vec3( 1 );
                material.metallic = 0.0;
                material.smoothness = 0.4;
                material.reflectance = 1.;
            }
#endif

            if ( material.type == 0 )
            {
                // PBR

                float E = 1.;
                float roughness = 1.0 - material.smoothness * material.smoothness;
                float alpha = roughness * roughness;
                float metallic = material.metallic;
                float reflectance = material.reflectance;
                float specular = material.specularAmount;
                float diffuse = 1.0 - specular;
                vec4 color = vec4( material.color * diffuse + material.specularColor * specular, 1.0 );

                vec3 brdf = vec3(0);

#if (PATH == 1)
                if ( random() < reflectance)
#else
                if ( true )
#endif
                {
                    vec3 brdf = sampleLightBRDF( hitOrigin, normal, ray.dir, material );

                    vec2 rand = rand2();
                    float xsi_1 = rand.x;
                    float xsi_2 = rand.y;
                    float phi = atan((alpha * sqrt(xsi_1)) / sqrt(1.0 - xsi_1));
                    float theta = 2.0 * PI * xsi_2;
                    vec3 direction = angleToDir(normal, theta, phi);

                    ray = Ray( hitOrigin, direction );

                    tcol += fcol * vec4( material.emission, 1.0 ) * E + fcol * color * vec4( brdf, 1.0 );
                    fcol *= color;
                } else
                {
                    float r2 = random();
                    vec3 d = jitter(normal, 2.*PI*random(), sqrt(r2), sqrt(1. - r2));

                    vec3 e = sampleLightE( hitOrigin, normal, ray.dir, material );

                    float E = 1.;

                    ray = Ray( hitOrigin, d );

                    tcol += fcol * vec4( material.emission, 1.0 ) * E + fcol * color * vec4( e, 1.0 );
                    fcol *= color;
                }
            } else
            if ( material.type == 1 )
            {
                // --- Dielectric - material.ior is the index of refraction
                // --- Based on this smallpt implementation https://www.shadertoy.com/view/MsySzd

                vec3 nl = dot(normal,ray.dir) < 0.0 ? normal : normal * -1.0;
                float specular = material.specularAmount;
                float diffuse = 1.0 - specular;
                vec4 color = vec4( material.color * diffuse + material.specularColor * specular, 1.0 );
                fcol *= color;

                // Ideal dielectric REFRACTION
                Ray reflRay = Ray( hitOrigin, ray.dir - normal * 2.0 * dot(normal,ray.dir));
                bool into = dot(normal,nl) > 0.0;                // Ray from outside going in?

                float nc = 1.0;   // IOR of air
                float nt = material.ior; // IOR of solid
                float nnt = into ? nc / nt : nt / nc;
                float ddn = dot(ray.dir , nl);
                float cos2t = 1.0 - nnt * nnt * (1.0 - ddn * ddn);

                if (cos2t < 0.0)    // Total internal reflection
                {
                    tcol += fcol * vec4( material.emission, 1.0);
                    ray = reflRay;
                } else {
                    vec3 tdir = normalize(ray.dir * nnt - normal * ((into ? 1.0 : -1.0) * (ddn * nnt + sqrt(cos2t))));

                    float a = nt - nc;
                    float b = nt + nc;
                    float R0 = a * a / (b * b);
                    float c = 1.0 - (into ? -ddn : dot(tdir,normal));
                    float Re = R0 + (1.0 - R0) * c * c * c * c * c;
                    float Tr = 1.0 - Re;
                    float P = .25 + .5 * Re;
                    float RP = Re / P;
                    float TP = Tr / (1.0 - P);

                    if( random() < P )
                    {
                        vec3 brdf = sampleLightBRDF( hitOrigin, normal, ray.dir, material );

                        ray = reflRay;
                        fcol = fcol * RP;
                        tcol += fcol * vec4( material.emission, 1.0 ) + fcol * vec4( brdf, 1.0 );

                    } else {
                        vec3 e = sampleLightE( hitOrigin, normal, ray.dir, material );

                        ray = Ray( hitOrigin, normalize( tdir ) );
                        fcol = fcol * TP;
                        tcol += fcol * vec4( material.emission, 1.0 ) + fcol * vec4( e, 1.0 );
                    }
                }

            } else
            if ( material.type == 2 )
            {
                // --- Light
                return vec4( material.emission, 1.0 );
            }
        } else {

            vec4 backColor = vec4( 0.322, 0.322, 0.322, 01.0 );
            return tcol + fcol * backColor;
        }
    }

    return tcol;
}

#if (SHAPE != 5)
vec3 uOrigin = vec3( 0.0, 0.0, 2.5 );
vec3 uLookAt = vec3( 0.0, 0.0, 0 );
#else
vec3 uOrigin = vec3( 0.08, 0.5, 2.8 );
vec3 uLookAt = vec3( 0.4, 0.7, 0 );
#endif

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    vec2 uv = fragCoord.xy / iResolution.xy;

    float ratio = iResolution.x / iResolution.y;
    vec2 pixelSize = vec2(1.0) / iResolution.xy;

    randv2 = fract(cos((uv.xy+uv.yx * vec2(1000.0,1000.0) ) + vec2(iTime)*10.0));

    // --- Camera

    const float fov = 80.0;
    float halfWidth = tan(radians(fov) * 0.5);
    float halfHeight = halfWidth / ratio;

    vec3 upVector = vec3(0.0, 1.0, 0.0);

    vec3 w = normalize(uOrigin - uLookAt);
    vec3 u = cross(upVector, w);
    vec3 v = cross(w, u);

    vec3 lowerLeft = uOrigin - halfWidth * u - halfHeight * v - w;
    vec3 horizontal = u * halfWidth * 2.0;
    vec3 vertical = v * halfHeight * 2.0;
    vec3 dir = lowerLeft - uOrigin;
    vec2 rand = rand2();

    dir += horizontal * (pixelSize.x * rand.x + uv.x);
    dir += vertical * (pixelSize.y * rand.y + uv.y);

    // ---

    vec3 color = getColor( Ray( uOrigin, normalize( dir ) ) ).xyz;
    color = clamp( color, -1., 1. );
    color += texture( iChannel0, uv).xyz;
    fragColor = vec4( color, 1.0 );
}

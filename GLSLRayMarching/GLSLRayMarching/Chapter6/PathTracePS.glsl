#version 330 core
#define PI 3.1415926547
in vec2 screenCoord;

uniform vec2 screenSize;

uniform sampler2D envMap;

uniform int sampleCount;

out vec4 FragColor;

///////////////////////////////////////////////////////////////////////////////
uint wang_seed;
uint wang_hash(uint seed)
{
    seed = (seed ^ uint(61)) ^ (seed >> uint(16));
    seed *= uint(9);
    seed = seed ^ (seed >> uint(4));
    seed *= uint(0x27d4eb2d);
    seed = seed ^ (seed >> uint(15));
    return seed;
}

void seedcore4(vec2 screenCoord)
{
	wang_seed = uint(screenCoord.x * screenSize.x + screenCoord.y * screenSize.x * screenSize.y);
}

float randcore4()
{
	wang_seed = wang_hash(wang_seed);

	return float(wang_seed) * (1.0 / 4294967296.0);
}

void seed(vec2 screenCoord)
{
	seedcore4(screenCoord);
}

float rand()
{
	return randcore4();
}

vec2 rand2()
{
	return vec2(rand(), rand());
}

vec3 rand3()
{
	return vec3(rand(), rand(), rand());
}

vec4 rand4()
{
	return vec4(rand(), rand(), rand(), rand());
}

///////////////////////////////////////////////////////////////////////////////
struct Ray {
    vec3 origin;
    vec3 direction;
}; 

struct Camera 
{
	vec3 origin;
	vec3 target;
	vec3 up;
	float vfov;
	float aspect;
}; 

////////////////////////////////////////////////////////////////////////////////////
Ray CameraGetRay(Camera camera, vec2 offset)
{
	float halfHeight = tan(camera.vfov / 2.0 * PI / 180.0);
	float halfWidth = camera.aspect * halfHeight;

	vec3 w = normalize(camera.origin - camera.target);
	vec3 u = normalize(cross(camera.up, w));
	vec3 v = normalize(cross(w, u));

	vec3 lower_left_corner = camera.origin - halfWidth * u - halfHeight * v - w;
	vec3 horizontal = 2.0 * halfWidth * u;
	vec3 vertical = 2.0 * halfHeight * v;

	Ray ray;
	ray.origin = camera.origin;
	ray.direction = lower_left_corner + offset.x * horizontal + offset.y * vertical - camera.origin;
	return ray;
}

////////////////////////////////////////////////////////////////////////////////////
#define MAX_MARCHING_STEPS 300
#define MAX_DISTANCE 1000.0
#define EPSILON 0.001

float sdSphere(vec3 p, float r) 
{
	return length(p) - r;
}

float sdBox( vec3 p, vec3 b )
{
	vec3 q = abs(p) - b;
	return length(max(q,0.0)) + min(max(q.x,max(q.y,q.z)),0.0);
}

float sdRoundBox( vec3 p, vec3 b, float r )
{
	vec3 q = abs(p) - b;
	return length(max(q,0.0)) + min(max(q.x,max(q.y,q.z)),0.0) - r;
}

float sdBoundingBox( vec3 p, vec3 b, float e )
{
	p = abs(p  )-b;
	vec3 q = abs(p+e)-e;
	return min(min(
		length(max(vec3(p.x,q.y,q.z),0.0))+min(max(p.x,max(q.y,q.z)),0.0),
		length(max(vec3(q.x,p.y,q.z),0.0))+min(max(q.x,max(p.y,q.z)),0.0)),
		length(max(vec3(q.x,q.y,p.z),0.0))+min(max(q.x,max(q.y,p.z)),0.0));
}

float sdTorus( vec3 p, vec2 t )
{
	vec2 q = vec2(length(p.xz)-t.x,p.y);
	return length(q)-t.y;
}

float sdCappedTorus(in vec3 p, in vec2 sc, in float ra, in float rb)
{
	p.x = abs(p.x);
	float k = (sc.y*p.x>sc.x*p.y) ? dot(p.xy,sc) : length(p.xy);
	return sqrt( dot(p,p) + ra*ra - 2.0*ra*k ) - rb;
}

float sdLink( vec3 p, float le, float r1, float r2 )
{
	vec3 q = vec3( p.x, max(abs(p.y)-le,0.0), p.z );

	return length(vec2(length(q.xy)-r1,q.z)) - r2;
}

float sdCylinder( vec3 p, vec3 c )
{
	return length(p.xz-c.xy)-c.z;
}

float sdCone( in vec3 p, in vec2 c, float h )
{
  // c is the sin/cos of the angle, h is height
  // Alternatively pass q instead of (c,h),
  // which is the point at the base in 2D
  vec2 q = h*vec2(c.x/c.y,-1.0);
    
  vec2 w = vec2( length(p.xz), p.y );
  vec2 a = w - q*clamp( dot(w,q)/dot(q,q), 0.0, 1.0 );
  vec2 b = w - q*vec2( clamp( w.x/q.x, 0.0, 1.0 ), 1.0 );
  float k = sign( q.y );
  float d = min(dot( a, a ),dot(b, b));
  float s = max( k*(w.x*q.y-w.y*q.x),k*(w.y-q.y)  );
  return sqrt(d)*sign(s);
}

float sdCone1( vec3 p, vec2 c, float h )
{
  float q = length(p.xz);
  return max(dot(c.xy,vec2(q,p.y)),-h-p.y);
}

float sdPlane( vec3 p, vec3 n, float h )
{
  // n must be normalized
  return dot(p,n) + h;
}

////////////
/*
float opElongate( in sdf3d primitive, in vec3 p, in vec3 h )
{
    vec3 q = p - clamp( p, -h, h );
    return primitive( q );
}

float opElongate( in sdf3d primitive, in vec3 p, in vec3 h )
{
    vec3 q = abs(p)-h;
    return primitive( max(q,0.0) ) + min(max(q.x,max(q.y,q.z)),0.0);
}

float opRound( in sdf3d primitive, float rad )
{
    return primitive(p) - rad
}

float opOnion( in float sdf, in float thickness )
{
    return abs(sdf)-thickness;
}

float opExtrusion( in vec3 p, in sdf2d primitive, in float h )
{
    float d = primitive(p.xy)
    vec2 w = vec2( d, abs(p.z) - h );
    return min(max(w.x,w.y),0.0) + length(max(w,0.0));
}

float opRevolution( in vec3 p, in sdf2d primitive, float o )
{
    vec2 q = vec2( length(p.xz) - o, p.y );
    return primitive(q)
}
*/

float opUnion( float d1, float d2 ) {  return min(d1,d2); }

float opSubtraction( float d1, float d2 ) { return max(-d1,d2); }

float opIntersection( float d1, float d2 ) { return max(d1,d2); }

float opSmoothUnion( float d1, float d2, float k ) {
    float h = clamp( 0.5 + 0.5*(d2-d1)/k, 0.0, 1.0 );
    return mix( d2, d1, h ) - k*h*(1.0-h); }

float opSmoothSubtraction( float d1, float d2, float k ) {
    float h = clamp( 0.5 - 0.5*(d2+d1)/k, 0.0, 1.0 );
    return mix( d2, -d1, h ) + k*h*(1.0-h); }

float opSmoothIntersection( float d1, float d2, float k ) {
    float h = clamp( 0.5 - 0.5*(d2-d1)/k, 0.0, 1.0 );
    return mix( d2, d1, h ) + k*h*(1.0-h); }

/*
vec3 opTx( in vec3 p, in transform t, in sdf3d primitive )
{
    return primitive( invert(t)*p );
}

float opScale( in vec3 p, in float s, in sdf3d primitive )
{
    return primitive(p/s)*s;
}
*/

float sdScene(vec3 p)
{
	//return opSmoothIntersection(sdSphere(p, 2.0), sdBox(p, vec3(3.0, 0.5, 3.0)), 0.5);
	//return opSmoothSubtraction(sdSphere(p, 2.0), sdBox(p, vec3(3.0, 0.5, 3.0)), 0.5);
	//return opSmoothUnion(sdSphere(p, 2.0), sdBox(p, vec3(3.0, 0.5, 3.0)), 0.5);

	//return opIntersection(sdSphere(p, 2.0), sdBox(p, vec3(3.0, 0.5, 3.0)));
	//return opSubtraction(sdSphere(p, 2.0), sdBox(p, vec3(3.0, 0.5, 3.0)));
	//return opUnion(sdSphere(p, 2.0), sdBox(p, vec3(3.0, 0.5, 3.0)));

	//return sdPlane(p, vec3(0, 1, 0), 0);
	//return sdCone1(p, vec2(0.3, 0.6), 1.0);
	//return sdCone(p, vec2(0.3, 0.6), 1.0);
	//return sdCylinder(p, vec3(0.0, 1.0, 0.9));
	//return sdLink(p, 1.5, 1.1, 0.3);
	//return sdCappedTorus(p, vec2(0.5, 0.5), 1.0, 0.3);
	//return sdTorus(p, vec2(1.0, 0.3));
	//return sdBoundingBox(p, vec3(0.5, 0.5, 0.5), 0.01);
	//return sdRoundBox(p, vec3(0.5, 0.5, 0.5), 0.1);
	//return sdBox(p, vec3(1.0, 1.0, 1.0));
	return sdSphere(p, 1.0);
}

float rayMarch(Ray ray, float start, float end)
{
	float h = 0.5;
	float t = 0.0;

	for (int i = 0; i < MAX_MARCHING_STEPS; ++i)
	{
		if (h < 0.001 || t > end)
		{
			break;
		}

		h = sdScene(ray.origin + t * ray.direction);
		t += h;
	}

	if (t > end)
	{
		t = -1.0;
	}

	return t;
}

vec3 estimateNormal(vec3 p) 
{
	vec3 eps = vec3(EPSILON, 0.0, 0.0);
	vec3 nor = vec3
	(
		sdScene(p + eps.xyy) - sdScene(p - eps.xyy),
		sdScene(p + eps.yxy) - sdScene(p - eps.yxy),
		sdScene(p + eps.yyx) - sdScene(p - eps.yyx)
	);

	return normalize(nor);
}

uniform Camera camera;

vec3 ambientLightColor = vec3(0.2, 0.2, 0.2);

vec3 lightColor0 = vec3(1.0, 1.0, 1.0);
vec3 lightDir0 = vec3(1, 1, 1);

vec3 materialDiffuseColor0 = vec3(0.8, 0.8, 0.8);
vec3 materialSpecularColor0 = vec3(1.0, 1.0, 1.0);
float materialGlossinessPower = 400.0;
float materialGlossinessStrength = 2.0;

vec3 AmbientShading(vec3 ambientLightColor, vec3 kA)
{
	vec3 ambient = ambientLightColor * kA;
	
	return ambient;
}

vec3 LambertShading(vec3 lightColor, vec3 kD, vec3 L, vec3 N)
{
	vec3 diffuse = lightColor * kD * max(dot(L, N), 0);
	
	return diffuse;
}

vec3 PhongShading(vec3 lightColor, vec3 kS, vec3 R, vec3 V, float glossinessPower, float glossinessStrength)
{
	vec3 specular = lightColor * kS * pow(max(dot(R, V), 0), glossinessPower) * glossinessStrength;

	return specular;
}

vec3 color(Camera camera, vec2 uv)
{
	Ray ray = CameraGetRay(camera, uv);

	float t = rayMarch(ray, 0.0, MAX_DISTANCE);
	if(t>=0)
	{
		vec3 O = ray.origin;
		vec3 P = ray.origin + ray.direction * t;

		vec3 Li0 = lightColor0;
		vec3 L = normalize(lightDir0);
		
		vec3 N = estimateNormal(P);
		vec3 V = normalize(O - P);
		vec3 R = reflect(-L, N);

		vec3 kA = materialDiffuseColor0;
		vec3 kD = materialDiffuseColor0;
		vec3 kS = materialSpecularColor0;
		
		float glossinessPower = materialGlossinessPower;
		float glossinessStrength = materialGlossinessStrength;

		vec3 ambient = AmbientShading(ambientLightColor, kA);
		vec3 diffuse = LambertShading(Li0, kD, L, N);
		vec3 specular = PhongShading(Li0, kS, R, V, glossinessPower, glossinessStrength);
		//return ambient;
		//return diffuse;
		//return specular;
		return ambient + diffuse + specular;
	}
	else
		return vec3(0.0, 0.0, 0.0);
}

void main()
{
	float u = screenCoord.x;
	float v = screenCoord.y;
	
	vec3 col = vec3(0.0, 0.0, 0.0);
	
	col += color(camera, vec2(u, v));

	FragColor.xyz = col;
	FragColor.w = 1.0;
}
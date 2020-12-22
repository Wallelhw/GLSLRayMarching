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
	vec3 v = cross(w, u);

	vec3 lower_left_corner = camera.origin - halfWidth * u - halfHeight * v - w;
	vec3 horizontal = 2.0 * halfWidth * u;
	vec3 vertical = 2.0 * halfHeight * v;

	Ray ray;
	ray.origin = camera.origin;
	ray.direction = lower_left_corner + offset.x * horizontal + offset.y * vertical - camera.origin;
	return ray;
}

////////////////////////////////////////////////////////////////////////////////////
#define MAX_MARCHING_STEPS 50
#define MAX_DISTANCE 1000.0
#define EPSILON 0.001

float sdSphere(vec3 p, float r) {
    return length(p) - r;
}

float sdScene(vec3 p)
{
	return sdSphere(p, 1.0);
}

float rayMarch(Ray ray, float start, float end)
{
	const float maxd = 5.0;

	float h = 0.5;
	float t = 0.0;

	for (int i = 0; i < MAX_MARCHING_STEPS; ++i)
	{
		if (h < 0.001 || t > maxd)
		{
			break;
		}

		h = sdScene(ray.origin + t * ray.direction);
		t += h;
	}

	if (t > maxd)
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

vec3 color(Camera camera, vec2 uv)
{
	Ray ray = CameraGetRay(camera, uv);

	float t = rayMarch(ray, 0.0, MAX_DISTANCE);
	if(t>=0)
	{
		vec3 O = ray.origin;
		vec3 P = ray.origin + ray.direction * t;
		vec3 N = estimateNormal(P);

		return N;
	}
	else
		return vec3(0.0, 0.0, 0.0);
}

uniform Camera camera;

void main()
{
	float u = screenCoord.x;
	float v = screenCoord.y;
	
	vec3 col = vec3(0.0, 0.0, 0.0);
	
	col += color(camera, vec2(u, v));

	FragColor.xyz = col;
	FragColor.w = 1.0;
}
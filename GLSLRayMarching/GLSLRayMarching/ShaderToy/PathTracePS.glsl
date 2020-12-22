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
#define MAX_MARCHING_STEPS 300
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

uniform Camera camera;

vec3 ambientLightColor = vec3(0.2, 0.2, 0.2);

vec3 lightColor0 = vec3(1.0, 1.0, 1.0);
vec3 lightDir0 = vec3(0, 0, 1);

vec3 materialDiffuseColor0 = vec3(0.8, 0.2, 0.2);
vec3 materialSpecularColor0 = vec3(0.3, 0.3, 0.3);
float materialGlossinessPower = 2.0;
float materialGlossiness = 1.0;

uniform float materialMetallic;
uniform float materialRoughness;
uniform float anisotropic;

float TrowbridgeReitzNormalDistribution(float NdotH, float roughness){
    float roughnessSqr = roughness*roughness;
    float Distribution = NdotH*NdotH * (roughnessSqr-1.0) + 1.0;
    return roughnessSqr / (3.1415926535 * Distribution*Distribution);
}

float PhongNormalDistribution(float RdotV, float specularpower, float speculargloss)
{
    float Distribution = pow(RdotV,speculargloss) * specularpower;

    Distribution *= (2+specularpower) / (2*3.1415926535);
    
	return Distribution;
}

float BlinnPhongNormalDistribution(float NdotH, float specularpower, float speculargloss){
    float Distribution = pow(NdotH,speculargloss) * specularpower;
    Distribution *= (2+specularpower) / (2*3.1415926535);
    return Distribution;
}

float BeckmannNormalDistribution(float roughness, float NdotH)
{
    float roughnessSqr = roughness*roughness;
    float NdotHSqr = NdotH*NdotH;
    return max(0.000001,(1.0 / (3.1415926535*roughnessSqr*NdotHSqr*NdotHSqr)) * exp((NdotHSqr-1)/(roughnessSqr*NdotHSqr)));
}

float GaussianNormalDistribution(float roughness, float NdotH)
{
    float roughnessSqr = roughness*roughness;
	float thetaH = acos(NdotH);
    return exp(-thetaH*thetaH/roughnessSqr);
}

float sqr(float v)
{
	return v * v;
}

float GGXNormalDistribution(float roughness, float NdotH)
{
    float roughnessSqr = roughness*roughness;
    float NdotHSqr = NdotH*NdotH;
    float TanNdotHSqr = (1-NdotHSqr)/NdotHSqr;
    return (1.0/3.1415926535) * sqr(roughness/(NdotHSqr * (roughnessSqr + TanNdotHSqr)));
}

float TrowbridgeReitzAnisotropicNormalDistribution(float anisotropic, float roughness, float NdotH, float HdotX, float HdotY)
{
    float aspect = sqrt(1.0-anisotropic * 0.9);
    float X = max(.001, sqr(roughness)/aspect) * 5;
    float Y = max(.001, sqr(roughness)*aspect) * 5;
    
    return 1.0 / (3.1415926535 * X*Y * sqr(sqr(HdotX/X) + sqr(HdotY/Y) + NdotH*NdotH));
}

float WardAnisotropicNormalDistribution(float anisotropic, float roughness, 
			float NdotL, float NdotV, float NdotH, float HdotX, float HdotY){
    float aspect = sqrt(1.0-anisotropic * 0.9);
    float X = max(.001, sqr(1.0-roughness)/aspect) * 5;
 	float Y = max(.001, sqr(1.0-roughness)*aspect) * 5;
    float exponent = -(sqr(HdotX/X) + sqr(HdotY/Y)) / sqr(NdotH);
    float Distribution = 1.0 / (4.0 * 3.14159265 * X * Y * sqrt(NdotL * NdotV));
    Distribution *= exp(exponent);
    return Distribution;
}

float ImplicitGeometricShadowingFunction (float NdotL, float NdotV)
{
	float Gs =  (NdotL*NdotV);       
	return Gs;
}

float AshikhminShirleyGSF (float NdotL, float NdotV, float LdotH)
{
	float Gs = NdotL*NdotV/(LdotH*max(NdotL,NdotV));
	return  (Gs);
}

float AshikhminPremozeGeometricShadowingFunction (float NdotL, float NdotV){
	float Gs = NdotL*NdotV/(NdotL+NdotV - NdotL*NdotV);
	return  (Gs);
}

float DuerGeometricShadowingFunction (vec3 lightDirection, vec3 viewDirection, vec3 normalDirection, float NdotL, float NdotV)
{
    vec3 LpV = lightDirection + viewDirection;
    float Gs = dot(LpV,LpV) * pow(dot(LpV,normalDirection),-4);
    return Gs;
}

float NeumannGeometricShadowingFunction (float NdotL, float NdotV){
	float Gs = (NdotL*NdotV)/max(NdotL, NdotV);       
	return  (Gs);
}

float KelemenGeometricShadowingFunction (float NdotL, float NdotV, 
float LdotV, float VdotH){
	float Gs = (NdotL*NdotV)/(VdotH * VdotH); 
	return   (Gs);
}

float WardGeometricShadowingFunction (float NdotL, float NdotV, 
float VdotH, float NdotH){
	float Gs = pow( NdotL * NdotV, 0.5);
	return  (Gs);
}

float KurtGeometricShadowingFunction (float NdotL, float NdotV, 
float VdotH, float roughness){
	float Gs =  NdotL*NdotV/(VdotH*pow(NdotL*NdotV, roughness));
	return  (Gs);
}

float CookTorrenceGeometricShadowingFunction (float NdotL, float NdotV, float VdotH, float NdotH){
	float Gs = min(1.0, min(2*NdotH*NdotV / VdotH, 2*NdotH*NdotL / VdotH));
	return  (Gs);
}

float SchlickGeometricShadowingFunction (float NdotL, float NdotV, float roughness)
{
    float roughnessSqr = roughness*roughness;

	float SmithL = (NdotL)/(NdotL * (1-roughnessSqr) + roughnessSqr);
	float SmithV = (NdotV)/(NdotV * (1-roughnessSqr) + roughnessSqr);

	return (SmithL * SmithV); 
}

float SchlickGGXGeometricShadowingFunction (float NdotL, float NdotV, float roughness){
    float k = roughness / 2;


    float SmithL = (NdotL)/ (NdotL * (1- k) + k);
    float SmithV = (NdotV)/ (NdotV * (1- k) + k);


	float Gs =  (SmithL * SmithV);
	return Gs;
}

float GGXGeometricShadowingFunction (float NdotL, float NdotV, float roughness)
{
    float roughnessSqr = roughness*roughness;
    float NdotLSqr = NdotL*NdotL;
    float NdotVSqr = NdotV*NdotV;

    float SmithL = (2 * NdotL)/ (NdotL + sqrt(roughnessSqr + ( 1-roughnessSqr) * NdotLSqr));
    float SmithV = (2 * NdotV)/ (NdotV + sqrt(roughnessSqr + ( 1-roughnessSqr) * NdotVSqr));

	float Gs =  (SmithL * SmithV);
	return Gs;
}

//------------------------------------------------
//schlick functions
float SchlickFresnel(float i)
{
    float x = clamp(1.0-i, 0.0, 1.0);
    
	float x2 = x * x;

    return x2 * x2 * x;
}

vec3 FresnelLerp(vec3 x, vec3 y, float d)
{
	float t = SchlickFresnel(d);	
	return mix(x, y, t);
}

vec3 SchlickFresnelFunction(vec3 SpecularColor, float NdotV)
{
    return SpecularColor + (vec3(1.0, 1.0, 1.0) - SpecularColor) * SchlickFresnel(NdotV);
}

float SchlickIORFresnelFunction(float ior, float LdotH)
{
    float f0 = pow((ior-1)/(ior+1),2);
    return f0 +  (1 - f0) * SchlickFresnel(LdotH);
}

float SphericalGaussianFresnelFunction(float LdotH, float SpecularColor)
{	
	float power = ((-5.55473 * LdotH) - 6.98316) * LdotH;
    return SpecularColor + (1 - SpecularColor)  * pow(2,power);
}

/*
float F0 (float NdotL, float NdotV, float LdotH, float roughness)
{
	float FresnelLight = SchlickFresnel(NdotL); 
    float FresnelView = SchlickFresnel(NdotV);
    float FresnelDiffuse90 = 0.5 + 2.0 * LdotH*LdotH * roughness;
	return MixFunction(1, FresnelDiffuse90, FresnelLight) * MixFunction(1, FresnelDiffuse90, FresnelView);
}
*/

vec3 color(Camera camera, vec2 uv)
{
	Ray ray = CameraGetRay(camera, uv);

	float t = rayMarch(ray, 0.0, MAX_DISTANCE);
	if(t>=0)
	{
		vec3 O = ray.origin;
		vec3 P = ray.origin + ray.direction * t;

		vec3 L  = normalize(lightDir0);
		
		vec3 N = estimateNormal(P);
		vec3 V = normalize(O - P);
		vec3 R = reflect(-L, N);
     
		vec3 V1 = reflect(-V, N);
		vec3 H  = normalize(V+L); 

		vec3 T = normalize(cross(N, vec3(0.0, -1.0, 0.0)));
		vec3 B = normalize(cross(T, N));

		float NdotL = max(dot(N, L), 0.0);
		float NdotH = max(dot(N, H), 0.0);
		float NdotV = max(dot(N, V), 0.0);
		float VdotH = max(dot(V, H), 0.0);
		float LdotH = max(dot(L, H), 0.0);
		float LdotV = max(dot(L, V), 0.0); 
		float RdotV = max(dot(R, V), 0.0);

		vec3 LColor0 = lightColor0;

		float kD = 1.0 - materialMetallic;
		float kS = materialMetallic;

		vec3 NormalDistribution = materialSpecularColor0;
		//NormalDistribution *= BlinnPhongNormalDistribution(NdotH, materialGlossinessPower,  max(1,materialGlossiness * 40));
		//NormalDistribution *= PhongNormalDistribution(RdotV, materialGlossinessPower, max(1, materialGlossiness * 100));
		//NormalDistribution *= BeckmannNormalDistribution(materialRoughness, NdotH);
		//NormalDistribution *= GaussianNormalDistribution(materialRoughness, NdotH);
		NormalDistribution *= GGXNormalDistribution(materialRoughness, NdotH);
		//NormalDistribution *= TrowbridgeReitzNormalDistribution(NdotH, materialRoughness);
		//NormalDistribution *= TrowbridgeReitzAnisotropicNormalDistribution(anisotropic, materialRoughness, NdotH, dot(H, T), dot(H, B));
		//NormalDistribution *= WardAnisotropicNormalDistribution(anisotropic, materialRoughness, NdotL, NdotV, NdotH, dot(H, T), dot(H, B));

		float GeometricShadow = 1.0;
		//GeometricShadow *= ImplicitGeometricShadowingFunction(NdotL, NdotV);
		//GeometricShadow *= AshikhminShirleyGSF(NdotL, NdotV, LdotH);
		//GeometricShadow *= AshikhminPremozeGeometricShadowingFunction (NdotL, NdotV);
		//GeometricShadow *= DuerGeometricShadowingFunction(L, V, N, NdotL, NdotV);
		//GeometricShadow *= NeumannGeometricShadowingFunction (NdotL, NdotV);
		//GeometricShadow *= KelemenGeometricShadowingFunction (NdotL, NdotV, LdotV, VdotH);
		//GeometricShadow *= WardGeometricShadowingFunction (NdotL, NdotV, VdotH, NdotH);
		//GeometricShadow *= KurtGeometricShadowingFunction (NdotL, NdotV, VdotH, materialRoughness);
		//GeometricShadow *= CookTorrenceGeometricShadowingFunction (NdotL, NdotV, VdotH, NdotH);
		//GeometricShadow *= SchlickGeometricShadowingFunction (NdotL, NdotV, materialRoughness);
		GeometricShadow *= SchlickGGXGeometricShadowingFunction (NdotL, NdotV, materialRoughness);
		//GeometricShadow *= GGXGeometricShadowingFunction (NdotL, NdotV, materialRoughness);

		vec3 FresnelFunction = materialSpecularColor0;
		FresnelFunction *= SchlickFresnelFunction(materialSpecularColor0, NdotV);

		vec3 diffuse  = kD * materialDiffuseColor0;
		vec3 specular = kS * FresnelFunction * GeometricShadow * NormalDistribution;
		return diffuse + specular;

		//return FresnelFunction;
		//return vec3(1, 1, 1) * GeometricShadow;
		//return vec3(1, 1, 1) * SpecularDistribution;
		//return vec3(1, 1, 1);
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
//Based on Naty Hoffmann and Arcot J. Preetham. Rendering out-door light scattering in real time.
//http://renderwonk.com/publications/gdm-2002/GDM_August_2002.pdf

#define fov tan(radians(60.0))

#define cameraheight 5e1 //50.

#define Gamma 2.2

#define Rayleigh 1.
#define Mie 1.
#define RayleighAtt 1.
#define MieAtt 1.2

//float g = -0.84;
//float g = -0.97;
float g = -0.9;

#if 1
vec3 _betaR = vec3(1.95e-2, 1.1e-1, 2.94e-1); 
vec3 _betaM = vec3(4e-2, 4e-2, 4e-2);
#else
vec3 _betaR = vec3(6.95e-2, 1.18e-1, 2.44e-1); 
vec3 _betaM = vec3(4e-2, 4e-2, 4e-2);
#endif


const float ts= (cameraheight / 2.5e5);

vec3 Ds = normalize(vec3(0., 0., -1.)); //sun 

vec3 ACESFilm( vec3 x )
{
    float tA = 2.51;
    float tB = 0.03;
    float tC = 2.43;
    float tD = 0.59;
    float tE = 0.14;
    return clamp((x*(tA*x+tB))/(x*(tC*x+tD)+tE),0.0,1.0);
}

void mainImage( out vec4 fragColor, in vec2 fragCoord ) {

	float AR = iResolution.x/iResolution.y;
    float M = 1.0; //canvas.innerWidth/M //canvas.innerHeight/M --res
    
    vec2 uvMouse = (iMouse.xy / iResolution.xy);
    uvMouse.x *= AR;
    
   	vec2 uv0 = (fragCoord.xy / iResolution.xy);
    uv0 *= M;
	//uv0.x *= AR;
    
    vec2 uv = uv0 * (2.0*M) - (1.0*M);
    uv.x *=AR;
    
    if (uvMouse.y == 0.) uvMouse.y=(0.7-(0.05*fov)); //initial view 
    if (uvMouse.x == 0.) uvMouse.x=(1.0-(0.05*fov)); //initial view
    
	Ds = normalize(vec3(uvMouse.x-((0.5*AR)), uvMouse.y-0.5, (fov/-2.0)));
    
	vec3 O = vec3(0., cameraheight, 0.);
	vec3 D = normalize(vec3(uv, -(fov*M)));

	vec3 color = vec3(0.);
    
	if (D.y < -ts) {
		float L = - O.y / D.y;
		O = O + D * L;
        D.y = -D.y;
		D = normalize(D);
	}
    else{
     	float L1 =  O.y / D.y;
		vec3 O1 = O + D * L1;

    	vec3 D1 = vec3(1.);
    	D1 = normalize(D);
    }

      // optical depth -> zenithAngle
      float sR = RayleighAtt / D.y ;
      float sM = MieAtt / D.y ;

  	  float cosine = clamp(dot(D,Ds),0.0,1.0);
      vec3 extinction = exp(-(_betaR * sR + _betaM * sM));

       // scattering phase
      float g2 = g * g;
      float fcos2 = cosine * cosine;
      float miePhase = Mie * pow(1. + g2 + 2. * g * cosine, -1.5) * (1. - g2) / (2. + g2);
        //g = 0;
      float rayleighPhase = Rayleigh;

      vec3 inScatter = (1. + fcos2) * vec3(rayleighPhase + _betaM / _betaR * miePhase);

      color = inScatter*(1.0-extinction); // *vec3(1.6,1.4,1.0)

        // sun
      color += 0.47*vec3(1.6,1.4,1.0)*pow( cosine, 350.0 ) * extinction;
      // sun haze
      color += 0.4*vec3(0.8,0.9,1.0)*pow( cosine, 2.0 )* extinction;
    
	  color = ACESFilm(color);
    
      color = pow(color, vec3(Gamma));
    
	  fragColor = vec4(color, 1.);
}
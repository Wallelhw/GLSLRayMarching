// create by JiepengTan 2018-04-18  email: jiepengtan@gmail.com

vec3 _BaseWaterColor = (vec3(22.,79.,86.)/255.);
vec3 _LightWaterColor= (vec3(0.,214.,10.)/255.);

const float SC = 10.;
float waterHeight = 100.;
float waterTranDeep = 10.;
vec3 lightDir = normalize( vec3(0.5,0.6,0.) );
const mat2 m2 = mat2( 0.60, -0.80, 0.80, 0.60 );
const mat3 m3 = mat3( 0.00,  0.80,  0.60,
               -0.80,  0.36, -0.48,
               -0.60, -0.48,  0.64 );


// from iq
float Noise( in vec3 x )
{
    vec3 p = floor(x);
    vec3 f = fract(x);
  	f = f*f*(3.0-2.0*f);
  	vec2 uv = (p.xy+vec2(37.0,17.0)*p.z) + f.xy;
  	vec2 rg = textureLod( iChannel0, (uv+0.5)/256.0, 0.0).yx;
  	return mix( rg.x, rg.y, f.z );
}

// ref https://www.shadertoy.com/view/Xs33Df
float Noise3D(in vec3 p){
    const vec3 s = vec3(7, 157, 113);
	vec3 ip = floor(p); // Unique unit cell ID.
    vec4 h = vec4(0., s.yz, s.y + s.z) + dot(ip, s);
	p -= ip; // Cell's fractional component.
    p = p*p*(3. - 2.*p);
    h = mix(fract(sin(h)*43758.5453), fract(sin(h + s.x)*43758.5453), p.x);
    h.xy = mix(h.xz, h.yw, p.y);
    return mix(h.x, h.y, p.z); // Range: [0, 1].
	
}

float FBM( in vec3 p )
{
    float n = 0.0;
    n += 0.50000*Noise( p*1.0 );
    n += 0.25000*Noise( p*2.0 );
    n += 0.12500*Noise( p*4.0 );
    n += 0.06250*Noise( p*8.0 );
    n += 0.03125*Noise( p*16.0 );
    return n/0.984375;
}
//ref: https://www.shadertoy.com/view/Msdfz8
vec3 Cloud(vec3 bgCol,vec3 ro,vec3 rd,vec3 cloudCol,float spd)
{
    vec3 col = bgCol;
    float t = iTime * 0.15* spd;
    vec2 sc = ro.xz + rd.xz*((3.)*40000.0-ro.y)/rd.y;
    vec2 p = 0.00002*sc;
    float f = 0.0;
  	float s = 0.5;
  	float sum =0.;
  	for(int i=0;i<5;i++){
    	p += t;t *=1.5;
    	f += s*textureLod( iChannel0, p/256.0, 0.0).x; p = m2*p*2.02;
    	sum+= s;s*=0.6;
  	}
    float val = f/sum; 
    col = mix( col, cloudCol, 0.5*smoothstep(0.5,0.8,val) );
    return col;
}

float WaterMap( vec3 pos ) {
    return FBM( vec3( pos.xz, iTime*0.3 )) * 1.;
}

vec3 WaterNormal(vec3 pos,float rz){
    float EPSILON =rz*rz* 0.002;
    vec3 dx = vec3( EPSILON, 0.,0. );
    vec3 dz = vec3( 0.,0., EPSILON );
      
    vec3  normal = vec3( 0., 1., 0. );
    float bumpfactor = 0.3 * pow(1.-clamp((rz)/1000.,0.,1.),6.);//
    
    normal.x = -bumpfactor * (WaterMap(pos + dx) - WaterMap(pos-dx) ) / (2. * EPSILON);
    normal.z = -bumpfactor * (WaterMap(pos + dz) - WaterMap(pos-dz) ) / (2. * EPSILON);
    return normalize( normal ); 
}

vec3 RayMarchCloud(vec3 ro,vec3 rd){
    vec3 col = vec3(0.0,0.0,0.0);  
    float sundot = clamp(dot(rd,lightDir),0.0,1.0);
    
     // sky      
    col = vec3(0.2,0.5,0.85)*1.1 - rd.y*rd.y*0.5;
    col = mix( col, 0.85*vec3(0.7,0.75,0.85), pow( 1.0-max(rd.y,0.0), 4.0 ) );
    // sun
    col += 0.25*vec3(1.0,0.7,0.4)*pow( sundot,5.0 );
    col += 0.25*vec3(1.0,0.8,0.6)*pow( sundot,64.0 );
    col += 0.4*vec3(1.0,0.8,0.6)*pow( sundot,512.0 );
    // clouds
    col = Cloud(col,ro,rd,vec3(1.0,0.95,1.0),1.);
            // .
    col = mix( col, 0.68*vec3(0.4,0.65,1.0), pow( 1.0-max(rd.y,0.0), 16.0 ) );
    return col;
}
float TerrainH( in vec2 x ) {
    
	vec2  p = x*0.03/SC;
    float a = 0.0;
    float b = 0.5;
	vec2  d = vec2(0.0);
    for( int i=0; i<9; i++ )
    {
        float n = Noise3D(vec3(p,0.));
        a += b*n;
		b *= 0.5;
        p *=m2* 2.0;
    }
	return SC*30.0*a;
}



float InteresctTerrial( in vec3 ro, in vec3 rd, in float tmin, in float tmax )
{
    float t = tmin;
    for( int i=0; i<256; i++ ) 
    {
        vec3 p = ro + t*rd;
        float h = p.y - TerrainH( p.xz );
        if( h<(0.002*t) || t>tmax ) break;
        t += 0.9*h;
    }
    return t; 
}

float SoftShadow(in vec3 ro, in vec3 rd )
{
    float res = 1.0;
    float t = 0.001;
    for( int i=0; i<80; i++ )
    {
        vec3  p = ro + t*rd;
        float h = p.y - TerrainH( p.xz );
        res = min( res, 16.0*h/t );
        t += h;
        if( res<0.001 ||p.y>(SC*20.0) ) break;
    }
    return clamp( res, 0.0, 1.0 );
}



vec3 CalcTerrianNormal( in vec3 pos, float t )
{
    vec2  eps = vec2( 0.002*t, 0.0 );
    return normalize( vec3( TerrainH(pos.xz-eps.xy) - TerrainH(pos.xz+eps.xy),
                            2.0*eps.x,
                            TerrainH(pos.xz-eps.yx) - TerrainH(pos.xz+eps.yx) ) );
}

vec3 RayMarchTerrial(vec3 ro,vec3 rd,float rz){
    vec3 col = vec3(0.,0.,0.);
    vec3 pos = ro + rz * rd;
    vec3 nor = CalcTerrianNormal(pos,rz);

    vec3 ref = reflect( rd, nor );
    float fre = clamp( 1.0+dot(rd,nor), 0.0, 1.0 );
    vec3 hal = normalize(lightDir-rd);
	col = vec3(0.08,0.05,0.03);
    // lighting     
    float amb = clamp(0.5+0.5*nor.y,0.0,1.0);
    float dif = clamp( dot( lightDir, nor ), 0.0, 1.0 );
    float bac = clamp( 0.2 + 0.8*dot( normalize( vec3(-lightDir.x, 0.0, lightDir.z ) ), nor ), 0.0, 1.0 );

    //shadow
    float sh = 1.0; 
  
    vec3 lin  = vec3(0.0,0.0,0.0);
    lin += dif*vec3(7.00,5.00,3.00)*1.3;
    lin += amb*vec3(0.40,0.60,1.00)*1.2;
    lin += bac*vec3(0.40,0.50,0.60);
    col *= lin;
  
    // fog
    float fo = 1.0-exp(-pow(0.001*rz/SC,1.5));
    vec3 fco = 0.65*vec3(0.4,0.65,1.0);// + 0.1*vec3(1.0,0.8,0.5)*pow( sundot, 4.0 );
    col = mix( col, fco, fo );
  return col;
}

#define mouse (iMouse.xy / iResolution.xy)
vec3 InitCam(in vec2 fragCoord ){
    float time = iTime;
    vec2 uv = fragCoord.xy / iResolution.xy;
  
  	vec2 p = fragCoord.xy/iResolution.xy-0.5;
    vec2 q = fragCoord.xy/iResolution.xy;
	p.x*=iResolution.x/iResolution.y;
    vec2 mo = iMouse.xy / iResolution.xy-.5;
    mo = (mo==vec2(-.5))?mo=vec2(-0,-0.03):mo;
	mo.x *= iResolution.x/iResolution.y * 3.14159;
	

    mo.x += smoothstep(0.6,1.,0.5+0.5)-1.5;
    vec3 eyedir = normalize(vec3(cos(mo.x),mo.y*2.-0.2+sin(1.57)*0.1,sin(mo.x)));
    vec3 rightdir = normalize(vec3(cos(mo.x+1.5708),0.,sin(mo.x+1.5708)));
    vec3 updir = normalize(cross(rightdir,eyedir));
	vec3 rd=normalize((p.x*rightdir+p.y*updir)*1.+eyedir);
	return rd;
}

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    float maxT = 10000.;
    float minT = 0.1;
    vec3 col  = vec3 (0.,0.,0.);
    float waterT = maxT;
    
    vec3 ro = vec3(-79.,104., -4.0);
	vec3 rd = InitCam(fragCoord);
	
    if(rd.y <-0.01){
      	float t = -(ro.y - waterHeight)/rd.y;
      	waterT = min(waterT,t);
    }
    float sundot = clamp(dot(rd,lightDir),0.0,1.0);

    float rz = InteresctTerrial(ro,rd,minT,maxT);
    float fresnel = 0.;
    vec3 refractCol = vec3(0.,0.,0.);
    bool reflected = false;
    
    // hit the water
    if(rz >= waterT && rd.y < -0.01){
        vec3 waterPos = ro + rd * waterT; 
        vec3 nor = WaterNormal(waterPos,waterT);
        float ndotr = dot(nor,-rd);
        fresnel = pow(1.0-abs(ndotr),6.);
        float diff = pow(dot(nor,lightDir) * 0.4 + 0.6,3.);
        // get the water col 
        vec3 waterCol = _BaseWaterColor + diff * _LightWaterColor * 0.12; 
        float transPer = pow(1.0-clamp( rz - waterT,0.,waterTranDeep)/waterTranDeep,3.);
        vec3 bgCol = RayMarchTerrial(ro,rd + nor* clamp(1.-dot(rd,-nor),0.,1.),rz);
        refractCol = mix(waterCol,bgCol,transPer);
		//reset the reflect dir and position
        ro = waterPos;
        rd = reflect( rd, nor);
        rz = InteresctTerrial(ro,rd,minT,maxT);
        reflected = true;
        col = refractCol;
    }
    if(rz >= maxT){
        col = RayMarchCloud( ro, rd);
    }else{
        col = RayMarchTerrial(ro,rd,rz);
    }
    if( reflected == true ) {
        col = mix(refractCol,col,fresnel);
        float spec=  pow(max(dot(rd,lightDir),0.0),128.) * 3.;
        col += vec3(spec,spec,spec);
    }
    
    fragColor = vec4(col,1.0);
}
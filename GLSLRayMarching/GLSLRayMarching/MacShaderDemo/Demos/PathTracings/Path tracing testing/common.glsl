#define hash21(p) fract(sin(dot(p, vec2(12.9898, 78.233))) * 43758.5453)

float sdBox( vec3 p, vec3 b ) {
    vec3 d = abs(p) - b;
    return min(max(d.x,max(d.y,d.z)),0.0) + length(max(d,0.0));
}

float sdSphere( vec3 p, float r ) {
    return length(p) - r;
}

float sdHexPrism( vec3 p, vec2 h )
{
    const vec3 k = vec3(-0.8660254, 0.5, 0.57735);
    p = abs(p);
    p.xy -= 2.0*min(dot(k.xy, p.xy), 0.0)*k.xy;
    vec2 d = vec2(
       length(p.xy-vec2(clamp(p.x,-k.z*h.x,k.z*h.x), h.x))*sign(p.y-h.x),
       p.z-h.y );
    return min(max(d.x,d.y),0.0) + length(max(d,0.0));
}

float opSmoothIntersection( float d1, float d2, float k ) {
    float h = clamp( 0.5 - 0.5*(d2-d1)/k, 0.0, 1.0 );
    return mix( d2, d1, h ) + k*h*(1.0-h); 
}

float sdLens( vec3 p, vec2 l, vec3 d ) {
    d = normalize(d);
    float r = (l.y*l.y-l.x*l.x)*0.5/l.x;
    float d1 = sdSphere(p+d*r,r+l.x);
    float d2 = sdSphere(p-d*r,r+l.x);
    return opSmoothIntersection(d1,d2,0.05);
}

struct object {
    int n;
    //vec3 color;
    float dist;
};// n = -1 is reserved for the sky

float sd = 0.5;

float rand() {
    //sd = hash21(vec2(sd,sd)+0.5);
   	sd = mod(mod(sd*25.398635864258,1.0)+0.23606797284447,1.0);
    //sd = mod(sd*12345.67890+0.1234567,1.0);
    return sd;
}

void seed(float n) {
    sd = n;
}

vec3 srand() {
    float u = rand()*2.0-1.0;
    float v = rand()*6.283;
    float d = sqrt(1.-u*u);
    return vec3(d*cos(v),d*sin(v),u);
}

vec3 brand(vec3 n) {
    vec3 s = vec3(-normalize(n.xz)*n.y,length(n.xz)).xzy;
    if (dot(n.xz,n.xz) < 0.01) {
        s = vec3(-normalize(n.xy)*n.z,length(n.xy));
    }
    vec3 r = cross(n,s);
    float u = rand()*6.283;
    float v = rand();
    vec3 d = s*cos(u)+r*sin(u);
    d *= sqrt(1.0-v);
    return n*sqrt(v)+d;
}


float smin( float a, float b, float k )
{
    float h = max( k-abs(a-b), 0.0 )/k;
    return min( a, b ) - h*h*k*(1.0/4.0);
}

vec2 uvfd(mat3 camera, vec3 d) {
    vec3 z = transpose(camera)*d;
    return z.xy/z.z;
}

vec3 refr(vec3 d, vec3 n, object o) {
    // this one assumes a lot of things, and is set to glass, i'll fix this later
    if (dot(d,n) > 0.0) {
        n = -n;
    }
    float eta = 1.52;
    if (o.dist > 0.0) {
        // outside the object
        eta = 1.0/eta;
    }// eta=pow(1.52,sign(o.dist)) ??? 
    vec3 r = refract(d,n,eta);
    if (r == vec3(0)) {
        return reflect(d,n);//that's what happens right?
    }
    return r;
}
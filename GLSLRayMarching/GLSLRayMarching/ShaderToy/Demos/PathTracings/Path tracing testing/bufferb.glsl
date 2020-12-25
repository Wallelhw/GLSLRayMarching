#define MAXITER 100
#define MAXDIST 30.0
#define MINDIST 0.0005
#define RAYDEPTH 42
#define intersect(n1,n2) ds[n1]=max(-ds[n2],ds[n1])

float ll(float a, float b) {
    return round(fract(atan(a/b)*3.0));
}

vec3 cmap(object o, vec3 p) {
    vec3[] colors = vec3[](
        vec3(0.7,0.7,0.9),
        //texture(iChannel2,p.xz).xyz,
        vec3(0.8,0.1,0.8),
        vec3(0.8,0.8,(ll(p.y,length(p.xz))+ll(p.x,p.z))*0.4+0.1),
        //texture(iChannel0,atan(vec2(p.y/length(p.xz),p.z/p.x))*2.0).xyz,
        //vec3(0.9,0.8,0.7),
        vec3(0.4,0.9,0.4),
        vec3(0.95,1,1),//vec3(round(fract(p.z))*0.7,1,1),
        //texture(iChannel0,p.zy/2.0).xyz,
        vec3(1),vec3(1)
    );
    return colors[o.n];
}

bool lmap(object o) {
    return o.n == 2;
}

bool rmap(object o) {
    return false;//o.n == 4;
}

bool dmap(object o) {
    return o.n == 4;
}

object map(vec3 p) {
    float[] ds = float[](
        sdSphere(p+vec3(0,42,0),42.0),//floor
        sdSphere(p-vec3(0,0.5,0),0.4),//small ball
        -sdSphere(p,8.0),//outside
        sdSphere(p-vec3(1,0.25,-0.5),0.9),//big ball
        abs(length(p.xz)-1.0)-0.1,//abs(p.x+0.2)-0.1,//wall,
        //sdLens(p-vec3(-0.5,0.7,-1),vec2(0.05,0.3),vec3(0.81,-0.28,0.85)),
        -sdSphere(p+vec3(0,1,0),2.0),1.0
		/* for reasons */
    );
    //ds[4] = max(-ds[1],ds[4]);
    //intersect(4,1);
    //intersect(4,0);
    intersect(4,5);
    intersect(3,4);
    //ds[3] = max(-ds[1],ds[3]);
    //intersect(3,1);
    //ds[1] = 100.0;
    ds[5] = 100.0;
    int n = 0;
    float d = ds[0];
    for (int i = 1; i < ds.length(); i++) {
        if (abs(ds[i]) < d) {
            d = abs(ds[i]);
            n = i;
        }
    }
    return object(n,ds[n]);
}

vec3 norm( vec3 p ) {
    vec3 up = vec3(0.0, 1.0, 0.0);
    vec3 fr = vec3(0.0, 0.0, 1.0);
    vec3 rt = vec3(1.0, 0.0, 0.0);
    
    float mp = map(p).dist;
    float mu = map(p + mp * up).dist;
    float md = map(p - mp * up).dist;
    float mr = map(p + mp * rt).dist;
    float ml = map(p - mp * rt).dist;
    float mf = map(p + mp * fr).dist;
    float mb = map(p - mp * fr).dist;
    
    //return normalize(vec3((mr-ml)/(mr+ml),(mu-md)/(mu+md),(mf-mb)/(mf+mb)));
    return normalize(vec3(mr-ml,mu-md,mf-mb));
}

object iterate(inout vec3 p, in vec3 d, out int iters) {
    float ad = 0.0;
    object m;
    bool ss = true;
    iters = MAXITER;
    for (int i = 0; i < MAXITER; i++) {
        m = map(p);
        float md = abs(m.dist);
        if (ad > MAXDIST) {
            iters = i;
            return object(-1,md); // hit the sky
        }
        if (md < MINDIST) {
            iters = i;
            return m;
        }
        ad += md;
        p += md*d;
        //d = normalize(d+brand(d)*0.01);
        //if (rand() < 0.0001) return object(-2,0.0);//hit a dust
    }
    return m; // hit ?
}

float focusl(vec3 eye, mat3 c) {
    float minres = min(iResolution.x,iResolution.y);
    vec2 im = iMouse.xy;
    if (im == vec2(0)) {
        im = vec2(.47,.49)*iResolution.xy;
    }
    vec2 uv = (im-0.5*iResolution.xy)/minres;
    vec3 d = c*normalize(vec3(uv,1));
    vec3 p = eye;
    int bob;
    object o = iterate(p,d,bob);
    return dot(p-eye,d);
}

vec3 drect(vec3 p, vec3 l, int on, vec3 n) {
    vec3 d = normalize(l-p);
    int bob;
    object o = iterate(p,d,bob);
    if (o.n == on) {
        return cmap(o,p)*dot(n,d);
    }
    return vec3(0);
}

vec4 trace(inout vec3 p, in vec3 d) {//, in mat3 cam) {
    vec3 c = vec3(1);
    vec3 direct = vec3(0);
    vec3 op = p;
   	//float fd = -1.0;
    //vec2 bob = iResolution.xy*0.5/min(iResolution.x,iResolution.y);
    //float n = texture(iChannel0,vec2(0.5)).z;
    //n = n/(1.0-n);
    float iters = 0.0;
    for (int i = 0; i < RAYDEPTH; i++) {
        int bob;
        op = p;
        object o = iterate(p,d,bob);
        vec3 cc = cmap(o,p);
        //if (o.dist < 0.0) {
        //    c *= exp(distance(p,op)*8.0*log(cc));
        //}
        //if (fd < 0.0) {
            //fd = distance(p,op);
            //iters = float(bob);
        //}
        if (lmap(o)) {
            return vec4(direct+c*cc,iters);
        }
        vec3 n = norm(p);
        if (dot(n,d) > 0.0)  {
            n = -n;
        }
        if (rmap(o)) {
            d = reflect(d,n);
        } else if (dmap(o)) {
            d = refr(d,n,o);
            if (dot(d,n) < 0.0) {//refracted
                n *= -1.0;
                p += MINDIST*2.0*n;//get to the other side
            }
        } else {
            d = brand(n);
            // if we can see a point, we can bootstrap off of what we see
            // (this does not work)
            //vec4 z = texture(iChannel1,uvfd(cam,p-op)+bob);
            //float dis = distance(p,op);
            //if (abs(dis-z.w) < 0.01) {
            //    cc = z.xyz*(1.0-n)+cc*n;
            //    //direct += c*z.xyz;
            //}
        }
        c *= cc;
        p += MINDIST*2.0*n;
        //direct += c*drect(p,vec3(0,0.5,0),1,n);
    }
    return vec4(direct,iters);
}

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    float n = texture(iChannel0,vec2(0.5)).z;
    //seed(sin(fragCoord.x-fragCoord.y)); // lines, slower
    seed(fragCoord.x/fragCoord.y); // noise, slowest (on my computer)
    seed(sin(0.01/n+rand()));
    fragCoord.x += rand(); fragCoord.y += rand();
    float minres = min(iResolution.x,iResolution.y);
    vec2 uv = (fragCoord-0.5*iResolution.xy)/minres;
    vec2 im = iMouse.xy/iResolution.xy;
    if (im == vec2(0)) {
        im = vec2(.47,.49);
    }
    im.x *= -6.283;
    im.x += 0.2;
    im.y = im.y*1.5+0.8;
    vec3 eye = vec3(3.0*vec2(sin(im.x),cos(im.x)),im.y).xzy;
    eye = normalize(eye)*2.5;
    //vec3 eye = vec3(3,0,0);
    vec3 fdir = normalize(vec3(0,0.5,0)-eye);
    vec3 udir = vec3(-normalize(fdir.xz)*fdir.y,length(fdir.xz)).xzy;
    vec3 rdir = cross(fdir,udir);
    mat3 cam = mat3(rdir,udir,fdir);
    vec3 d = eye + focusl(eye,cam)*(fdir*1.0+udir*uv.y+rdir*uv.x);
    float rrr = rand()*2.0*3.1416;
    //eye += 0.0225*(sin(rrr)*rdir+cos(rrr)*udir)*sqrt(rand());
    eye += 0.05*(rand()*rdir+rand()*udir);
    d = normalize(d-eye);
    //d = cam*normalize(vec3(uv,1));
    /*
	vec3 z = vec3(0);
    int n = 10;
    for (int i = 0; i < n; i++) {
        z += trace(eye,d);
    }
    z /= float(n);
    fragColor = vec4(z,0);/*/
    //vec3 e = eye;
    fragColor = trace(eye,d);
  	//fragColor = texture(iChannel1,uvfd(cam,eye-e)+iResolution.xy*0.5/minres);
    /* bob */
}
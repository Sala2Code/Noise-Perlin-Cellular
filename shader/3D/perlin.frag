#version 330 core

uniform vec2 u_resolution;
uniform float u_time;
uniform float u_seed;
uniform float u_zoom;
uniform float u_blur; 
uniform float u_brightness; 
uniform float u_highWave;
uniform float u_zoomWave;
uniform float u_detail;
uniform float u_blur2;

// uniform bool u_blob;
uniform bool u_wood;
uniform bool u_patatoid;

// * Utils
vec3 random3(vec3 coord) {   
    float rand = u_seed*4096.0*sin(dot(coord,vec3(17, 59, 15))); // pseudo random
    vec3 r;
    // Multiply each time by a factor to not make "pattern" ; Remove them and look 
    r.z = fract(rand);
    rand *= .175;
    r.x = fract(rand);
    rand *= .891;
    r.y = fract(rand);
    return (r-.5)*u_blur; // [0;1] -> [-0.5*blur;0.5*blur]
}
mat3 rotate3d(float angle){ // rotate matrix
    return mat3(
        cos(angle),-sin(angle),0.,
        sin(angle),cos(angle),0.,
        0., 0., 1.
                );
}

// * Effects
float noise(vec3 p) {
    vec3 i = floor(p);
    vec3 f = fract(p);

    // interpolation ; x-> 6x^5 − 15x^4 + 10x^3 works too
    vec3 u = f*f*(3.0-2.0*f);
    // interpolation between 8 corners
    return mix( 
            mix( // z = 0
                mix( // y = 0
                    dot( random3(i + vec3(0.0,0.0,0.0) ), f - vec3(0.0,0.0,0.0) ),
                    dot( random3(i + vec3(1.0,0.0,0.0) ), f - vec3(1.0,0.0,0.0) ), 
                    u.x),
                mix( // y = 1
                    dot( random3(i + vec3(0.0,1.0,0.0) ), f - vec3(0.0,1.0,0.0) ),
                    dot( random3(i + vec3(1.0,1.0,0.0) ), f - vec3(1.0,1.0,.0) ), 
                    u.x), 
                u.y),
                
            mix( // z = 1
                mix( // y = 0
                    dot( random3(i + vec3(0.0,0.0,1.0) ), f - vec3(0.0,0.0,1.0) ),
                    dot( random3(i + vec3(1.0,0.0,1.0) ), f - vec3(1.0,0.0,1.0) ), 
                    u.x),
                mix( // y= 1
                    dot( random3(i + vec3(0.0,1.0,1.0) ), f - vec3(0.0,1.0,1.0) ),
                    dot( random3(i + vec3(1.0,1.0,1.0) ), f - vec3(1.0,1.0,1.0) ), 
                    u.x), 
                u.y),

            u.z);
}

float lines(in vec3 pos, float blur){
    // return step(0., sin(pos.x)); // rougher
    return smoothstep(0., blur, sin(pos.x));
}


void main() {
    vec2 st = gl_FragCoord.xy/u_resolution.xy;
    st *= u_zoom;
    vec3 pos = vec3(st, u_time);
    float color;
    
    if(u_wood){ // wood planck // use blur is deprecated, it's ugly
        pos = rotate3d( noise(pos)*u_highWave+.5) * pos; // you can add some val to turn to another direction
        color = lines(pos*u_zoomWave, 1./(4.*u_brightness) ); // draw lines 
    }else if(u_patatoid){ // patatoid & nested circle ()
        color = fract(length(noise(pos))*u_detail);
        color = smoothstep(u_brightness-u_blur2, u_brightness, color);
    }else{ // basic perlin
        color = noise(pos) + u_brightness;
    }

    gl_FragColor = vec4(vec3(color),1.0);
}
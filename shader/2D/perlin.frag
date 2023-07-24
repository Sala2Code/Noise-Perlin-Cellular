#version 330 core

uniform vec2 u_resolution;
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
// 2D Random
vec2 random2(vec2 st){
    // scalar product yields pseudo random
    st = vec2(dot(st,vec2(17.478,31.057) * u_seed),
              dot(st,vec2(26.792,183.112)) * u_seed);
    return -1.0 + 2.0*u_blur*fract(sin(st)*430.498); // [0;1] -> [-blur;blur]
}
mat2 rotate2d(float angle){ // rotate matrix
    return mat2(cos(angle),-sin(angle),
                sin(angle),cos(angle));
}


// Effects
float noise (in vec2 st) {
    vec2 i = floor(st);
    vec2 f = fract(st);

    // x -> -2x^3 + 3x^2 or x-> 6x^5 − 15x^4 + 10x^3.
    vec2 u = f*f*(3.0-2.0*f);

    return mix( 
            mix( // interpolation selon les x (puis les y) des points de la grille et du point dans la carré avec le décalage (vecteur pointant f) 
                dot( random2(i + vec2(0.0,0.0) ), f - vec2(0.0,0.0) ),
                dot( random2(i + vec2(1.0,0.0) ), f - vec2(1.0,0.0) ),
                u.x),

            mix( 
                dot( random2(i + vec2(0.0,1.0) ), f - vec2(0.0,1.0) ),
                dot( random2(i + vec2(1.0,1.0) ), f - vec2(1.0,1.0) ), 
                u.x), 
            
            u.y);
}
float lines(in vec2 pos, float blur){
    // return step(0., sin(pos.x)); // rougher
    return smoothstep(0., blur, sin(pos.x));
}

void main() {
    vec2 st = gl_FragCoord.xy/u_resolution.xy;
    vec2 pos = vec2(st*u_zoom);
    
    float color;
    
    if(u_wood){ // wood planck // use blur is deprecated, it's ugly
        pos = rotate2d( noise(pos)*u_highWave+.5) * pos;
        color = lines(pos*u_zoomWave, 1./(4.*u_brightness) ); // draw lines 
    }else if(u_patatoid){
        color = fract(length(noise(pos))*u_detail);
        color = smoothstep(u_brightness-u_blur2, u_brightness, color);


    }else{ // basic perlin
        color = noise(pos) + u_brightness;
    }

    gl_FragColor = vec4(vec3(color), 1.0);
}


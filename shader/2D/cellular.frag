#version 330 core

#define PI 3.1415926535

uniform vec2 u_resolution;
// uniform float u_time;
uniform float u_seed;
uniform float u_zoom;
// uniform float u_blur; 
uniform float u_size;

uniform bool u_blob;


vec2 random2( vec2 st ) {
    return fract(sin(vec2(dot(st,vec2(127.1,311.7)),dot(st,vec2(269.5,183.3))))*43758.5453*u_seed);
}

void main() {
    vec2 st = gl_FragCoord.xy/u_resolution.xy;
    st*=u_zoom;
   
    vec3 color = vec3(.0);
    vec2 i_st = floor(st);
    vec2 f_st = fract(st);

    float m_dist = 1.;  // minimum distance
    for (int y= -1; y <= 1; y++) {
        for (int x= -1; x <= 1; x++) {
            vec2 neighbor = vec2(float(x),float(y));
            vec2 point = random2(i_st + neighbor);
            point = 0.5+0.5*sin( 2.*PI*point);

            float dist = length(neighbor + point - f_st);
            m_dist = (u_blob) ? min(m_dist, dist*m_dist) : min(m_dist, dist);
        }
    }
    color += (u_blob) ? step(m_dist, u_size) : m_dist;

    gl_FragColor = vec4(color,1.0);
}

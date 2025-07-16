#version 330 core

out vec4 FragColor;

uniform vec2 resolution;  
uniform int MaxIterations = 300;
uniform float zoom;
uniform vec2 center; 
uniform float aspect;

vec3 virdis_color_map(float t) {

    const vec3 c0 = vec3(0.2777273272234177, 0.005407344544966578, 0.3340998053353061);
    const vec3 c1 = vec3(0.1050930431085774, 1.404613529898575, 1.384590162594685);
    const vec3 c2 = vec3(-0.3308618287255563, 0.214847559468213, 0.09509516302823659);
    const vec3 c3 = vec3(-4.634230498983486, -5.799100973351585, -19.33244095627987);
    const vec3 c4 = vec3(6.228269936347081, 14.17993336680509, 56.69055260068105);
    const vec3 c5 = vec3(4.776384997670288, -13.74514537774601, -65.35303263337234);
    const vec3 c6 = vec3(-5.435455855934631, 4.645852612178535, 26.3124352495832);

    return c0+t*(c1+t*(c2+t*(c3+t*(c4+t*(c5+t*c6)))));

}

void main(){

    vec2 uv = (gl_FragCoord.xy/resolution - .5) *  vec2(aspect, 1.0);
    vec2 z = uv * zoom + center;

    vec2 c = vec2(-0.52, 0.52);
    
    int iterations = 0;

    for (int i = 0; i < MaxIterations; i++) {
        iterations++;

        z = vec2(
            z.x * z.x - z.y * z.y,
            2 * z.x * z.y
        );
        z += c;

        if((abs(z.x) + abs(z.y)) > 4.0){
            break;
        }
    }

    if(iterations == MaxIterations){
        FragColor = vec4(0.0, 0.0, 0.0, 1.0);
    } else{
        float zn = dot(z, z);
        float log_zn = log(zn) / 2.0;
        float nu = log(log_zn / log(2.0)) / log(2.0);
        float smooth_i = float(iterations) + 1.0 - nu;

        float color = smooth_i / MaxIterations;
        // FragColor = vec4(color,color,color,color);

        FragColor = vec4(virdis_color_map(color), 1.0);
    }


    // FragColor = vec4(1 - iterations/MaxIterations, 1- iterations/MaxIterations, 1- iterations/MaxIterations, 1- iterations/MaxIterations);
    
    //float smoothIteration = float(iterations) - log2(log(length(z))) + 4.0;

   
}
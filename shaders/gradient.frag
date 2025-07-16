#version 330 core

out vec4 FragColor;

uniform vec2 resolution;  

void main(){
    vec3 horizontal = mix(vec3(1.0, 1.0, 1.0), 
        vec3(0.4353, 0.0, 1.0),       
        gl_FragCoord.x/resolution.x);       

    vec3 vertical = mix(vec3(1.0, 0.4, 0.0), 
        vec3(0.0, 0.0, 1.0),       
        gl_FragCoord.y/resolution.y);    

    vec3 color = mix(horizontal, vertical, 0.5);

    FragColor = vec4(color, 1.0);
}
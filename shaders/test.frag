#version 330 core

out vec4 FragColor;

uniform vec2 u_resolution;  

void main(){
    vec3 horizontal = mix(vec3(1.0, 1.0, 1.0), 
        vec3(0.0, 0.0, 1.0),       
        gl_FragCoord.x/u_resolution.x);       

    vec3 vertical = mix(vec3(1.0, 0.0, 0.0), 
        vec3(0.0, 0.0, 1.0),       
        gl_FragCoord.y/u_resolution.y);    

    vec3 color = mix(horizontal, vertical, 0.5);

    FragColor = vec4(color, 1.0);
}
#version 430 core

flat in vec4 color;
uniform float timeShader;
out vec4 out_color;

void main() {
    float mixing = abs(sin(timeShader*0.2)) ;
    vec4 background = vec4(0.0, 0.3, 0.3, 1.0);
    vec4 randomColor = vec4(0.9, 0.2, 0.4, 1.0);
    out_color = mix(randomColor, background, mixing);  
    
   
}
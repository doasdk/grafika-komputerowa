#version 430 core

flat in vec4 color;
uniform float timeShader;
out vec4 out_color;

void main() {
    out_color=color;
          
}
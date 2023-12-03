#version 430 core

out vec4 out_color;

uniform vec3 colorFrag;

void main()
{
	out_color = vec4(colorFrag,1);
}

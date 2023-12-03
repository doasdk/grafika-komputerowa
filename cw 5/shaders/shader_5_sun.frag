#version 430 core

uniform vec3 color;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 cameraPos;

float M_PI = 3.1415926535897932384626433832795;

in vec3 interpNormal;
in vec3 vertexNormalOut;
in vec3 vertexPosOut;
out vec4 outColor;

void main()
{
	vec3 normalizedVertexNormal = normalize(vertexNormalOut);
	vec3 viewDir = normalize(cameraPos - vertexPosOut);
	float angle = dot(viewDir, normalizedVertexNormal);

	//outColor = vec4(color * (angle / M_PI), 1.0);
	outColor = vec4(mix(color, vec3(1), angle), 1.0);
}
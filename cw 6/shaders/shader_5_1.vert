#version 430 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 vertexTexCoord;

uniform mat4 transformation;
uniform mat4 modelMat;

out vec3 vertexNormalOut;
out vec3 vertexPosOut;

void main()
{
	gl_Position = transformation * vec4(vertexPosition, 1.0);
	vec4 worldNormal = modelMat * vec4(vertexNormal, 0.0);
	vertexNormalOut = worldNormal.xyz;
	vertexPosOut = (modelMat * vec4(vertexPosition, 1.0)).xyz;
}
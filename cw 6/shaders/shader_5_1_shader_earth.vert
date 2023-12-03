#version 430 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 vertexTexCoord;

uniform mat4 transformation;
uniform mat4 modelMatrix;

out vec2 vertexCoordinatesOfTex;

out vec3 vecNormal;
out vec3 worldPos;

void main()
{
	//vertexCoordinatesOfTex = vertexTexCoord;
	

	worldPos = (modelMatrix* vec4(vertexPosition,1)).xyz;
	vecNormal = (modelMatrix* vec4(vertexNormal,0)).xyz;
	vertexCoordinatesOfTex = vec2(vertexTexCoord.x, vertexTexCoord.y);
	gl_Position = transformation * vec4(vertexPosition, 1.0);
}

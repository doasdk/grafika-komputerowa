#version 430 core

float AMBIENT = 0.1;


uniform vec3 lightPos;

in vec3 vecNormal;
in vec3 worldPos;

uniform sampler2D colorTexture;

in vec2 vertexCoordinatesOfTex;
out vec4 outColor;

void main()
{

	vec4 textureColor = texture2D(colorTexture, vertexCoordinatesOfTex);

	vec3 lightDir = normalize(lightPos-worldPos);
	vec3 normal = normalize(vecNormal);
	float diffuse=max(0,dot(normal,lightDir));
	outColor = vec4(textureColor.rgb * min(1,AMBIENT+diffuse), 1.0)*9;
}

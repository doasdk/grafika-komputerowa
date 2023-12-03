#version 430 core

float AMBIENT = 0.1;


uniform vec3 lightPos;

in vec3 vecNormal;
in vec3 worldPos;

uniform sampler2D colorTexture;
uniform sampler2D clouds;

in vec2 vertexCoordinatesOfTex;
out vec4 outColor;

void main()
{

	vec4 earthColor = texture2D(colorTexture, vertexCoordinatesOfTex);
	vec4 cloudsColor = texture2D(clouds, vertexCoordinatesOfTex);

	vec3 lightDir = normalize(lightPos-worldPos);
	vec3 normal = normalize(vecNormal);
	float diffuse=max(0,dot(normal,lightDir));


	vec3 finalColor = mix(vec3(1.0), earthColor.rgb, cloudsColor.r); 

	outColor = vec4(finalColor * min(1,AMBIENT+diffuse), 1.0)*9;



}

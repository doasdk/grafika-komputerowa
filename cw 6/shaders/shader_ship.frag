#version 430 core

float AMBIENT = 0.1;


uniform vec3 lightPos;

in vec3 vecNormal;
in vec3 worldPos;

uniform sampler2D colorTexture;
uniform sampler2D rust;
uniform sampler2D scratches;

in vec2 vertexCoordinatesOfTex;
out vec4 outColor;

void main()
{

	vec4 shipColor = texture2D(colorTexture, vertexCoordinatesOfTex);
	vec4 rustColor = texture2D(rust, vertexCoordinatesOfTex);
	vec4 scratchesColor = texture2D(scratches, vertexCoordinatesOfTex);

	vec3 lightDir = normalize(lightPos-worldPos);
	vec3 normal = normalize(vecNormal);
	float diffuse=max(0,dot(normal,lightDir));

	vec3 rustMix = mix(vec3(1.0), shipColor.rgb, rustColor.r);
	vec3 finalColor = mix(rustMix, shipColor.rgb, scratchesColor.r); 

	outColor = vec4(finalColor * min(1,AMBIENT+diffuse), 1.0)*9;



}

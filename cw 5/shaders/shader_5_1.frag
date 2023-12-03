#version 430 core

uniform vec3 color;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 cameraPos;

uniform float time;
uniform float lightExp;

in vec3 vertexNormalOut;
in vec3 vertexPosOut;
out vec4 outColor;


void main()
{
	vec3 lightDir = normalize(vertexPosOut - lightPos);

	float lightDistance = length(vertexPosOut - lightPos);
	vec3 newLightColor = lightColor / pow(lightDistance, 2);
	
	vec3 normalizedVertexNormal = normalize(vertexNormalOut);
	float intensity = dot(normalizedVertexNormal, -lightDir);
	intensity  = max(intensity, 0.0);
	
	vec3 viewDir = normalize(cameraPos - vertexPosOut);
	vec3 reflectDir = reflect(lightDir, normalizedVertexNormal);
	
	float glossPow = 8;
	float specular = pow(max(dot(viewDir, reflectDir), 0.0), glossPow);

	float diffuse = intensity;
	outColor = vec4(newLightColor * (color * diffuse + specular ), 1.0);
	outColor = 1 - exp(-outColor * lightExp);
}

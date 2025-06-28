#version 460 core

struct Light
{
	vec3 position;
	vec3 color;
};

struct Material
{
	vec3 color;
};

layout(location = 0) in vec2 v_vertexTextureCoordinate;
layout(location = 1) in vec3 v_vertexNormal;
layout(location = 2) in vec3 v_fragmentPosition;

out vec4 color;

uniform sampler2D oTexture;

uniform Light oLight;
uniform Material oMaterial;
uniform vec3 oViewPosition;

void main()
{
	vec3 norm = normalize(v_vertexNormal);
	vec3 lightDirection = normalize(oLight.position - v_fragmentPosition);

	float diff = max(dot(norm, lightDirection), 0.0);
	vec3 diffuse = diff * oLight.color;

	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * oLight.color;
	
	float specularStrength = 0.23;
	vec3 viewDirection = normalize(oViewPosition - v_fragmentPosition);
	vec3 reflectionDirection = reflect(-lightDirection, norm);

	float spec = pow(max(dot(viewDirection, reflectionDirection), 0.0), 32);
	vec3 specular = specularStrength * spec * oLight.color;

	vec4 result = vec4((ambient + diffuse + specular) * oMaterial.color, 1);

	color = texture(oTexture, v_vertexTextureCoordinate) * result; 
}
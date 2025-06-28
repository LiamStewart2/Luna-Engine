#version 460 core

struct Light
{
	vec3 position;
	vec3 direction;
	vec3 color;
};

struct Material
{
	vec3 color;
};

layout(location = 0) in vec2 v_vertexTextureCoordinate;
layout(location = 1) in vec3 v_vertexNormal;
layout(location = 2) in vec3 v_fragmentPosition;
layout(location = 3) in vec4 v_fragmentPositionLightSpace;

out vec4 color;

uniform sampler2D diffuseTexture;
uniform sampler2D shadowMap;

uniform Light oLight;
uniform Material oMaterial;
uniform vec3 oViewPosition;

float ShadowCalculation(vec4 fragPosLightSpace)
{
	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    // If the fragment is outside the light's orthographic projection
    if (projCoords.z > 1.0 || projCoords.x < 0.0 || projCoords.x > 1.0 || projCoords.y < 0.0 || projCoords.y > 1.0)
        return 0.0;

    float closestDepth = texture(shadowMap, projCoords.xy).r;
    float currentDepth = projCoords.z;

    // Bias to prevent shadow acne
    float bias = max(0.005 * (1.0 - dot(normalize(v_vertexNormal), normalize(oLight.position - v_fragmentPosition))), 0.001);

    float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;

    return shadow;
}

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

	float shadow = ShadowCalculation(v_fragmentPositionLightSpace);

	vec4 result = vec4((ambient + (1.0 - shadow) * (diffuse + specular)) * oMaterial.color, 1);

	color = texture(diffuseTexture, v_vertexTextureCoordinate) * result;
}
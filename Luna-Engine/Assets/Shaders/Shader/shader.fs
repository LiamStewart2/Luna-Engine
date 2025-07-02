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

out vec4 color;

uniform sampler2D diffuseTexture;
uniform sampler2DArray shadowMap;

uniform mat4 view;
uniform float farPlane;

layout (std140) uniform LightSpaceMatrices
{
    mat4 lightSpaceMatrices[16];
};
uniform float cascadePlaneDistances[16];
uniform int cascadeCount;

uniform Light oLight;
uniform Material oMaterial;
uniform vec3 oViewPosition;

uniform int debugMode;

int selectedCascade = -1;

float rand(vec2 co)
{
	return fract(sin(dot(co.xy, vec2(12.9898, 78.233))) * 43758.5453);
}

float ShadowCalculation(vec3 fragmentPositionWorldSpace)
{
	// Select cascade layer
	vec4 fragmentPositionViewSpace = view * vec4(fragmentPositionWorldSpace, 1.0);
	float depthValue = -fragmentPositionViewSpace.z;

	int layer = -1;
	for(int i = 0; i < cascadeCount; ++i)
	{
		if(depthValue < cascadePlaneDistances[i])
		{
			selectedCascade = i;
			layer = i;
			break;
		}
	}
	if(layer == -1)
	{
		layer = cascadeCount;
		selectedCascade = cascadeCount;
	}


	vec4 fragmentPositionLightSpace = lightSpaceMatrices[layer] * vec4(fragmentPositionWorldSpace, 1.0);

	vec3 projCoords = fragmentPositionLightSpace.xyz / fragmentPositionLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    // If the fragment is outside the light's orthographic projection
    if (projCoords.z > 1.0 || projCoords.x < 0.0 || projCoords.x > 1.0 || projCoords.y < 0.0 || projCoords.y > 1.0)
        return 0.0;

    float currentDepth = projCoords.z;
	if(currentDepth > 1.0)
		return 0.0;

    // Bias to prevent shadow acne
    float bias = max(0.05 * (1.0 - dot(normalize(v_vertexNormal), normalize(oLight.direction))), 0.005);
	const float biasModifier = 0.5;

	if(layer == cascadeCount)
		bias *= 1 / (farPlane * biasModifier);
	else
		bias *= 1 / (cascadePlaneDistances[layer] * biasModifier);

	float shadow = 0.0;
	vec2 texelSize = 1.0 / vec2(textureSize(shadowMap, 0));
	for(int x = -1; x <= 1; ++x)
	{
		for(int y = -1; y <= 1; ++y)
		{
			vec2 coordSample = vec2(x, y);
			vec2 offset = vec2(rand(v_vertexTextureCoordinate.xy) * 1.3, rand(v_vertexTextureCoordinate.xy + vec2(1, 1)) * 1.3);

			vec2 sampleOffset = coordSample + offset;

			coordSample.x = sqrt(sampleOffset.x) * cos(6.283 * sampleOffset.y);
			coordSample.y = sqrt(sampleOffset.x) * sin(6.283 * sampleOffset.y);

			float pcfDepth = texture(shadowMap, vec3(projCoords.xy + offset * texelSize, layer)).r;
			shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
		}    
	}
	shadow /= 15.0;

    return shadow;
}

void main()
{
	vec3 norm = normalize(v_vertexNormal);
	vec3 lightDirection = normalize(oLight.direction);

	float diff = max(dot(norm, lightDirection), 0.0);
	vec3 diffuse = diff * oLight.color;

	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * oLight.color;
	
	float specularStrength = 0.3;
	vec3 viewDirection = normalize(oViewPosition - v_fragmentPosition);
	vec3 reflectionDirection = reflect(-lightDirection, norm);

	float spec = pow(max(dot(norm, normalize(lightDirection + viewDirection)), 0.0), 32);
	vec3 specular = specularStrength * spec * oLight.color;

	float shadow = ShadowCalculation(v_fragmentPosition);

	vec4 result = vec4((ambient + (1.0 - shadow) * (diffuse + specular)) * oMaterial.color, 1);

	color = texture(diffuseTexture, v_vertexTextureCoordinate) * result;

	if(debugMode == 1)
	{
		vec3 debugColor;
		if (selectedCascade == 0) debugColor = vec3(1, 1, 0);           // Yellow
		else if (selectedCascade == 1) debugColor = vec3(0, 1, 0);      // Green
		else if (selectedCascade == 2) debugColor = vec3(0, 0, 1);      // Blue
		else if (selectedCascade == 3) debugColor = vec3(1, 0, 0);      // Red
		else                          debugColor = vec3(1, 0, 1);      // Magenta (fallback)

		color *= vec4(debugColor, 1.0);
	}
}
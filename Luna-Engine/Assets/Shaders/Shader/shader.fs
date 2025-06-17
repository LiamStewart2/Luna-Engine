#version 460 core

layout(location = 0) in vec2 v_vertexTextureCoordinate;
layout(location = 1) in vec3 v_vertexNormal;
layout(location = 2) in vec3 v_fragmentPosition;

out vec4 color;

uniform sampler2D oTexture;

uniform vec3 oLightPosition;
uniform vec3 oLightColor;
uniform vec3 oObjectColor;

void main()
{
	vec3 norm = normalize(v_vertexNormal);
	vec3 lightDirection = normalize(oLightPosition - v_fragmentPosition);

	float diff = max(dot(norm, lightDirection), 0.0);
	vec3 diffuse = diff * oLightColor;

	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * oLightColor;
	
	vec4 result = vec4((ambient + diffuse) * oObjectColor, 1);

	color = texture(oTexture, v_vertexTextureCoordinate) * result;
}
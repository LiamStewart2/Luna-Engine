#version 460 core

layout(location = 0) in vec2 v_vertexTextureCoordinate;
layout(location = 1) in vec3 v_vertexNormal;

out vec4 color;

uniform sampler2D oTexture;

uniform vec3 oLightColor;
uniform vec3 oObjectColor;

void main()
{
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * oLightColor;
	
	vec4 result = vec4(ambient * oObjectColor, 1);

	color = texture(oTexture, v_vertexTextureCoordinate) * result;
}
#version 460 core

layout(location = 0) in vec2 v_vertexTextureCoordinate;
layout(location = 1) in vec3 v_vertexNormal;

out vec4 color;

uniform sampler2D oTexture;

void main()
{
	color = texture(oTexture, v_vertexTextureCoordinate) * vec4(1, 1, 1, 1);
}
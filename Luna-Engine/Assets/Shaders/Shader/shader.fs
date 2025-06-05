#version 460 core

layout(location = 0) in vec2 v_vertexTextureCoordinate;
layout(location = 1) in vec3 v_vertexNormal;

out vec4 color;

void main()
{
	color = vec4(1.0, 1.0, 1.0, 1.0);
}
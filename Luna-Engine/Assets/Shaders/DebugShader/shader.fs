#version 460 core

layout(location = 0) in vec2 v_vertexTextureCoordinate;

uniform sampler2D textureSample;

out vec4 color;

void main()
{
    color = texture(textureSample, v_vertexTextureCoordinate);
}
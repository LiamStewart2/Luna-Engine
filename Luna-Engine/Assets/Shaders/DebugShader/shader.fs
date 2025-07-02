#version 460 core

uniform vec3 lineColor;

out vec4 FragColor;

void main()
{
    FragColor = vec4(lineColor, 1.0);
}
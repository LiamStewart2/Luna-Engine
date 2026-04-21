#version 460 core

layout (location = 0) in vec3 v_vertexPosition;

uniform mat4 model;

void main()
{
    gl_Position = model * vec4(v_vertexPosition, 1.0);
}
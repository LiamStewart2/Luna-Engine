#version 460 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec2 vertexTextureCoordinate;
layout(location = 2) in vec3 vertexNormal;

layout(location = 0) out vec2 v_vertexTextureCoordinate;
layout(location = 1) out vec3 v_vertexNormal;
layout(location = 2) out vec3 v_fragmentPosition;
layout(location = 3) out vec4 v_fragmentPositionLightSpace;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix;

void main()
{
    gl_Position = projection * view * model * vec4(vertexPosition, 1.0);
    v_fragmentPosition = vec3(model * vec4(vertexPosition, 1.0));

    v_vertexTextureCoordinate = vertexTextureCoordinate;
    v_vertexNormal = vertexNormal;

    v_fragmentPositionLightSpace = lightSpaceMatrix * vec4(v_fragmentPosition, 1.0);
}
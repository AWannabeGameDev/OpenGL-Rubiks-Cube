#version 460 core

uniform mat4 u_projectionMatrix;
uniform mat4 u_modelMatrix;
uniform mat4 u_viewMatrix;

layout(location = 0) in vec3 i_position;
layout(location = 1) in vec4 i_color;

out vec4 vertexColor;

void main()
{
	gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * vec4(i_position, 1.0f);
	vertexColor = i_color;
}
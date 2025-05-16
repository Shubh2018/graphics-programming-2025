#version 410

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;

out vec4 vColor;
out vec3 vNormal;
out vec3 FragPos;

uniform vec4 meshColor;
uniform mat4 model;
uniform mat4 ViewProjectionMatrix;

void main()
{
	gl_Position = ViewProjectionMatrix * model * vec4(pos, 1.0f); 
	vColor = meshColor;
}
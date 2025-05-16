#version 410

in vec4 vColor;
out vec4 color;

in vec3 vNormal;
in vec3 FragPos;

uniform vec3 lightPos;
uniform vec4 lightColor;

uniform vec3 viewPos;

float ambientStrength = 0.1f;
float specularStrength = 0.5f;

void main()
{
	color = vColor;
}
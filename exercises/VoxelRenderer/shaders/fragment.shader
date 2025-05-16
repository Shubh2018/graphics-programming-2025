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


vec4 CalculateAmbientLight(float strength, vec4 lightColor)
{
	vec4 ambient = strength * lightColor;
	vec4 result = ambient * vColor;
	return result;
}

vec4 CalculateDiffuseLight(vec3 normal, vec3 lightPos, vec3 fragPos, vec4 lightColor, vec3 lightDir)
{
	float diff = max(dot(normal, lightDir), 0.0f);
	vec4 diffuse = diff * lightColor;
	return diffuse;
}

vec4 CalculateSpecularLight(vec3 normal, vec3 viewPos, vec3 lightDir, float specularStrength, vec4 lightColor)
{
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 32);
	vec4 specular = specularStrength * spec * lightColor;

	return specular;
}

void main()
{
	vec3 norm = normalize(vNormal);
	vec3 lightDir = normalize(lightPos - FragPos);

	vec4 ambient = CalculateAmbientLight(ambientStrength, lightColor);
	vec4 diffuse = CalculateDiffuseLight(norm, lightPos, FragPos, lightColor, lightDir);
	vec4 specular = CalculateSpecularLight(norm, viewPos, lightDir, specularStrength, lightColor);
	
	vec4 fragColor = (ambient + diffuse + specular) * vColor;
	color = fragColor;
}
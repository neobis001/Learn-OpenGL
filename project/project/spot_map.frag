#version 330 core

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

out vec4 color;
  
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 viewPos;
uniform sampler2D diffuse; //had to put it outside Material struct, else error
uniform sampler2D specular;

struct Material {
	float shininess;
};

struct Light {
	vec3 position;
	vec3 direction;
	float innerCutoff;
	float outerCutoff;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

uniform Material material;
uniform Light light;

void main()
{
	vec3 ambient = light.ambient * vec3(texture(diffuse, TexCoords));
    vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.position - FragPos);
	float theta = dot(lightDir, normalize(-light.direction));

	/*
	if (theta > light.outerCutoff)  {
		float diff = max(dot(norm, lightDir), 0.0);
		vec3 diffuse = light.diffuse * diff * vec3(texture(diffuse, TexCoords));

		float specularStrength = 0.5f;
		vec3 viewDir = normalize(viewPos - FragPos);
		vec3 reflectDir = reflect(-lightDir, norm);
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
		vec3 specular = light.specular * spec * vec3(texture(specular, TexCoords)); 


		float distance = length(light.position - FragPos);
		float attenuation = 1 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

		//ambient *= attenuation;
		diffuse *= attenuation;
		specular *= attenuation;

		float theta = dot(lightDir, normalize(-light.direction));
		float epsilon = light.innerCutoff - light.outerCutoff; 
		float intensity = clamp ((theta - light.outerCutoff) / epsilon, 0.0, 1.0);

		diffuse *= intensity;
		specular *= intensity;

		vec3 result = vec3(ambient + diffuse + specular);
		color = vec4(result, 1.0f);	
	} else {
		color = vec4(light.ambient * vec3(texture(diffuse, TexCoords)), 1.0f);
	}
	*/

	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * vec3(texture(diffuse, TexCoords));

	float specularStrength = 0.5f;
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = light.specular * spec * vec3(texture(specular, TexCoords)); 


	float distance = length(light.position - FragPos);
	float attenuation = 1 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	ambient *= attenuation; //can include this here now because no else statement where ambient is default
	diffuse *= attenuation;
	specular *= attenuation;

	theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.innerCutoff - light.outerCutoff; 
	float intensity = clamp ((theta - light.outerCutoff) / epsilon, 0.0, 1.0);

	diffuse *= intensity;
	specular *= intensity;

	vec3 result = vec3(ambient + diffuse + specular);
	color = vec4(result, 1.0f);	

}
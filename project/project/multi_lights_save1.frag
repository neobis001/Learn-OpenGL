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

struct DirLight {
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight {
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;  

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct TestLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;
    
	vec3 ambient;
    float test;
	vec3 specular;
};


uniform Material material;
uniform Light light;
uniform DirLight dirLight;
//uniform PointLight pointLights[4];

uniform TestLight testLight1;
uniform TestLight testLight2;
uniform TestLight testLight3;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);  

void main()
{
    vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);

	vec3 result = CalcDirLight(dirLight, norm, viewDir);
	//result += CalcPointLight(testLight, norm, FragPos, viewDir);
	vec3 testing = testLight1.position;
	testing = testLight2.position;
	testing = testLight3.position;

	/*
	for (int i = 0; i < NR_POINT_LIGHTS; i++) {
		result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
	}
	*/

	//result += CalcPointLight(pointLights[0], norm, FragPos, viewDir);

	/*
	vec3 testPosition = pointLights[0].position;
	float coefficient = pointLights[0].constant;
	coefficient = pointLights[0].linear;
	coefficient = pointLights[0].quadratic;
	vec3 testFactor = pointLights[0].ambient;
	testFactor = pointLights[0].diffuse;
	testFactor = pointLights[0].specular;

	
	testPosition = pointLights[1].position;
	coefficient = pointLights[1].constant;
	coefficient = pointLights[1].linear;
	coefficient = pointLights[1].quadratic;
	testFactor = pointLights[1].ambient;
	testFactor = pointLights[1].diffuse;
	testFactor = pointLights[1].specular;

	
	testPosition = pointLights[2].position;
	coefficient = pointLights[2].constant;
	coefficient = pointLights[2].linear;
	coefficient = pointLights[2].quadratic;
	testFactor = pointLights[2].ambient;
	testFactor = pointLights[2].diffuse;
	testFactor = pointLights[2].specular;

	
	testPosition = pointLights[3].position;
	coefficient = pointLights[3].constant;
	coefficient = pointLights[3].linear;
	coefficient = pointLights[3].quadratic;
	testFactor = pointLights[3].ambient;
	testFactor = pointLights[3].diffuse;
	testFactor = pointLights[3].specular;
	*/


	/*
	vec3 ambient = light.ambient * vec3(texture(diffuse, TexCoords));

	
	vec3 lightDir = normalize(light.position - FragPos);
	float theta = dot(lightDir, normalize(-light.direction));

	
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * vec3(texture(diffuse, TexCoords));

	float specularStrength = 0.5f;
	
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

	*/

	color = vec4(result, 1.0f);	
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // Combine results
    vec3 ambient  = light.ambient  * vec3(texture(diffuse, TexCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(specular, TexCoords));
    return (ambient + diffuse + specular);
}  

// Calculates the color when using a point light.
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // Attenuation
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0f / (light.constant + light.linear * distance + 
  			     light.quadratic * (distance * distance));    
    // Combine results
    vec3 ambient  = light.ambient  * vec3(texture(diffuse, TexCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(specular, TexCoords));
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
} 
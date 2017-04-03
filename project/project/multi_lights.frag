#version 330 core

out vec4 color;

struct PointLight {
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;  

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform PointLight pointLights[5];
//pointLights[4]; works, 5 crashes program

void main()
{
	vec3 testPositionValue = pointLights[3].position; //made this assignment
	  //to make sure pointLights uniform stays existent

	vec3 result = vec3(0.4); //default gray to objects drawn
	color = vec4(result, 1.0f);	
}
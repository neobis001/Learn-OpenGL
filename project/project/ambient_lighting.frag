#version 330 core
out vec4 color;
  
uniform vec3 objectColor;
uniform vec3 lightColor;

void main()
{
	float ambientStrength = .1;
	vec3 ambient = ambientStrength * lightColor;

	vec3 result = ambient * objectColor;
    color = vec4(result, 1.0f);
}
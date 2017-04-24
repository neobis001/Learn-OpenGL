#version 330 core
in vec2 TexCoords;

out vec4 color;

uniform sampler2D texture1;

void main()
{             
    //color = texture(texture1, TexCoords);
	//color = vec4(0.4f, 0.6f, 0.6f, 1.0f);
	color = texture(texture1, TexCoords);
	if (color.a < 0.1) {
		discard;
	}
}
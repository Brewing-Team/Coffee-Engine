#version 450 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D screenTexture;

void main()
{
	vec3 color = texture(screenTexture, TexCoord).rgb;

    FragColor = vec4(vec3(color), 1.0);
}

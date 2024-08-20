#version 450 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D screenTexture;
uniform float exposure;

void main()
{
    float gamma = 2.2;

    vec3 color = texture(screenTexture, TexCoord).rgb;

	color *= exposure/(1. + color / exposure);
    
    FragColor = vec4(pow(color, vec3(1.0 / gamma)), 1.0);
}

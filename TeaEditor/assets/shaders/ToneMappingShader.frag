#version 450 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D screenTexture;

void main()
{
    float gamma = 2.2;

    vec3 color = texture(screenTexture, TexCoord).rgb;
    
    FragColor = vec4(pow(color, vec3(1.0 / gamma)), 1.0);
}

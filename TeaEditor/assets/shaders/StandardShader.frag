#version 450 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D albedo;

void main()
{
    FragColor = texture(albedo, TexCoord);
}

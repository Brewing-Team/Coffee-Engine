
#version 450 core
out vec4 FragColor;

in vec2 TexCoord;

void main()
{
    FragColor = vec4(TexCoord.r, 1 - TexCoord.g, 0.0, 1.0);
}

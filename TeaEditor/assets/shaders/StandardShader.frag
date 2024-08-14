#version 450 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

in vec3 viewPos;

uniform sampler2D albedo;

void main()
{
    vec3 norm = normalize(Normal);

    vec3 viewDir = normalize(viewPos - FragPos);

    float diff = max(dot(norm, viewDir), 0.0);

    vec3 shading = diff * texture(albedo, TexCoord).rgb;

    FragColor = vec4(shading, 1.0f);
}

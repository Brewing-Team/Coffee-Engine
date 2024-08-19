#version 450 core
out vec4 FragColor;

#define MAX_LIGHTS 4

struct Light
{
    vec3 color;
    vec3 direction;
    vec3 position;
    int type;
};

layout (std140, binding = 1) uniform RenderData
{
    Light lights[MAX_LIGHTS];
    int lightCount;
};

in vec2 TexCoord;
in vec3 Normal;

in vec3 FragPos;
in vec3 camPos;

uniform sampler2D albedo;

void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(camPos - FragPos);

    // Directional light calculation
/*     vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(norm, lightDir), 0.0);
     vec3 shading = diff * light.color;

    FragColor = vec4(shading, 1.0f); */

    vec3 shading;

    for(int i = 0; i < lightCount; i++)
    {
        vec3 lightDir = normalize(-lights[i].direction);
        float diff = max(dot(norm, lightDir), 0.0);
        shading += diff * lights[i].color;
    }

    FragColor = vec4(shading, 1.0f);
}

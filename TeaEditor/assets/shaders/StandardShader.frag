#version 450 core
out vec4 FragColor;

uniform sampler2D albedo;
uniform sampler2D normal;
uniform sampler2D metallic;
uniform sampler2D roughness;
uniform sampler2D ao;
uniform sampler2D emissive;

#define MAX_LIGHTS 4

struct Light
{
    vec3 color;
    vec3 direction;
    vec3 position;

    float range;
    float attenuation;
    float intensity;

    float angle;

    int type;
};

layout (std140, binding = 1) uniform RenderData
{
    Light lights[MAX_LIGHTS];
    int lightCount;
};

in vec2 TexCoord;
in vec3 Normal;
in mat3 TBN;

in vec3 FragPos;
in vec3 camPos;

void main()
{
    vec3 norm /* = normalize(Normal) */;

    norm = texture(normal, TexCoord).rgb;
    norm = norm * 2.0 - 1.0;
    norm = normalize(TBN * norm);

    vec3 viewDir = normalize(camPos - FragPos);

    vec3 shading;

    for(int i = 0; i < lightCount; i++)
    {
        if(lights[i].type == 0)
        {
            // Directional Light
            vec3 lightDir = normalize(-lights[i].direction);
            float diff = max(dot(norm, lightDir), 0.0);

            diff *= lights[i].intensity;

            // Sample the albedo texture
            vec3 albedo = texture(albedo, TexCoord).rgb;

            // Calculate final shading
            shading += diff * lights[i].color * albedo;
        }
        else if(lights[i].type == 1)
        {
            //PointLight

            vec3 lightDir = normalize(lights[i].position - FragPos);
            float diff = max(dot(norm, lightDir), 0.0);
            float distance = length(lights[i].position - FragPos);

            // Calculate attenuation (Revise this function bc it should be like in godot but I doubt it)
            float attenuation = 1.0 / (1.0 + lights[i].attenuation * pow(distance / lights[i].range, 2));
            attenuation *= lights[i].intensity;

            // Apply attenuation to diffuse component
            diff *= attenuation;

            // Sample the albedo texture
            vec3 albedo = texture(albedo, TexCoord).rgb;

            // Calculate final shading
            shading += diff * lights[i].color * albedo;
        }
    }

    float gamma = 2.2;
    
    FragColor = vec4(pow(shading, vec3(1.0 / gamma)), 1.0);
}

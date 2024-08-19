#version 450 core
out vec4 FragColor;

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

in vec3 FragPos;
in vec3 camPos;

uniform sampler2D albedo;

void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(camPos - FragPos);

    vec3 shading;

    for(int i = 0; i < lightCount; i++)
    {
        if(lights[i].type == 0)
        {
            //Directional Light
            vec3 lightDir = normalize(-lights[i].direction);
            float diff = max(dot(norm, lightDir), 0.0);
            shading += diff * lights[i].color;
        }
        else if(lights[i].type == 1)
        {
            //PointLight
            vec3 lightDir = normalize(lights[i].position - FragPos);
            float diff = max(dot(norm, lightDir), 0.0);
            float distance = length(lights[i].position - FragPos);
            //Revise this function bc it should be like in godot but I doubt it
            float a = 1 / (1 + lights[i].attenuation * pow(distance / lights[i].range, 2));
            a *= lights[i].intensity;
            diff *= a;
            shading += diff * lights[i].color;
        }
    }

    FragColor = vec4(shading, 1.0f);
}

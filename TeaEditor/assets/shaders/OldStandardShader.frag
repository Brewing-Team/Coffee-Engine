#version 450 core
out vec4 FragColor;

uniform sampler2D albedo;
uniform sampler2D normal;
uniform sampler2D metallic;
uniform sampler2D roughness;
uniform sampler2D ao;
uniform sampler2D emissive;

#define MAX_LIGHTS 32

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

//https://github.com/godotengine/godot/blob/da5f39889f155658cef7f7ec3cc1abb94e17d815/modules/lightmapper_rd/lm_compute.glsl#L388
float getOmniAttenuation(float distance, float inv_range, float decay) {
	float nd = distance * inv_range;
	nd *= nd;
	nd *= nd; // nd^4
	nd = max(1.0 - nd, 0.0);
	nd *= nd; // nd^2
	return nd * pow(max(distance, 0.0001), -decay);
}

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

            //Thanks Godot for the PointLight math

            vec3 lightDir = normalize(lights[i].position - FragPos);
            //float diff = max(dot(norm, lightDir), 0.0);
            float distance = distance(FragPos, lights[i].position);

            if(distance > lights[i].range)
                continue;

            // Calculate attenuation (Revise this function bc it should be like in godot but I doubt it)
            //float attenuation = 1.0 / (1.0 + lights[i].attenuation * pow(distance / lights[i].range, 2))
            //float attenuation = lights[i].range / (distance * distance + lights[i].attenuation);
            //float attenuation = 1 / (distance * distance);
            //For now this is the winner!!!
            //float attenuation = lights[i].intensity / (1.0 + lights[i].attenuation * pow(distance / lights[i].range, 2));

            float attenuation = getOmniAttenuation(distance, 1.0 / lights[i].range, lights[i].attenuation);

            attenuation *= max(0.0, dot(norm, lightDir));

            if(attenuation <= 0.0001)
                continue;

            // Apply attenuation to diffuse component
            //diff *= attenuation;

            // Sample the albedo texture
            vec3 albedo = texture(albedo, TexCoord).rgb;

            // Calculate final shading
            //shading += diff * lights[i].color * albedo;
            shading += lights[i].color * lights[i].intensity * albedo * attenuation;
        }
    }

    FragColor = vec4(shading, 1.0);
}

#version 450 core
layout(location = 0) out vec4 FragColor;
layout(location = 1) out vec4 EntityID;

uniform vec3 entityID;

struct VertexData
{
    vec2 TexCoords;
    vec3 Normal;
    vec3 WorldPos;
    vec3 camPos;
    mat3 TBN;
};

layout (location = 2) in VertexData VertexInput;

/*Todo: Think if we should use a struct for the material or mimmic the Material
        structs in the C++ code.
*/
struct Material
{
    sampler2D albedoMap;
    sampler2D normalMap;
    sampler2D metallicMap;
    sampler2D roughnessMap;
    sampler2D aoMap;
    sampler2D emissiveMap;

    vec4 color;
    float metallic;
    float roughness;
    float ao;
    vec3 emissive;

    int hasAlbedo;
    int hasNormal;
    int hasMetallic;
    int hasRoughness;
    int hasAO;
    int hasEmissive;
};

uniform Material material;

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

const float PI = 3.14159265359;

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;

    float num = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return num / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float num = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return num / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}


void main()
{
    vec3 albedo = material.hasAlbedo * (texture(material.albedoMap, VertexInput.TexCoords).rgb * material.color.rgb) + (1 - material.hasAlbedo) * material.color.rgb;

    // Revise this type of conditional assignment (the commented one) because i think can lead to some undefined behavior in the shader!!!!!
    vec3 normal/*  = material.hasNormal * (VertexInput.TBN * (texture(material.normalMap, VertexInput.TexCoords).rgb * 2.0 - 1.0)) + (1 - material.hasNormal) * VertexInput.Normal */;
    if (material.hasNormal == 1) {
        normal = VertexInput.TBN * (texture(material.normalMap, VertexInput.TexCoords).rgb * 2.0 - 1.0);
    } else {
        normal = VertexInput.Normal;
    }
    float metallic = material.hasMetallic * (texture(material.metallicMap, VertexInput.TexCoords).b * material.metallic) + (1 - material.hasMetallic) * material.metallic;
    float roughness = material.hasRoughness * (texture(material.roughnessMap, VertexInput.TexCoords).g * material.roughness) + (1 - material.hasRoughness) * material.roughness;
    float ao = material.hasAO * (texture(material.aoMap, VertexInput.TexCoords).r * material.ao) + (1 - material.hasAO) * material.ao;
    vec3 emissive = material.hasEmissive * (texture(material.emissiveMap, VertexInput.TexCoords).rgb * material.emissive) + (1 - material.hasEmissive) * material.emissive;

    vec3 N = normalize(normal);
    vec3 V = normalize(VertexInput.camPos - VertexInput.WorldPos);

    vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedo, metallic);

    vec3 Lo = vec3(0.0);
    for(int i = 0; i < lightCount; i++)
    {
        vec3 L = vec3(0.0);

        vec3 radiance = vec3(0.0);

        if(lights[i].type == 0)
        {
            /*====Directional Light====*/

            L = normalize(-lights[i].direction);
            radiance = lights[i].color * lights[i].intensity;
        }
        else if(lights[i].type == 1)
        {
            /*====Point Light====*/

            L = normalize(lights[i].position - VertexInput.WorldPos);
            float distance = length(lights[i].position - VertexInput.WorldPos);
            float attenuation = 1.0 / (distance * distance);
            radiance = lights[i].color * attenuation * lights[i].intensity;
        }
        else if(lights[i].type == 2)
        {
            /*====Spot Light====*/
            
        }

        vec3 H = normalize(V + L);

        float NDF = DistributionGGX(N, H, roughness);
        float G = GeometrySmith(N, V, L, roughness);
        vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);

        vec3 kS = F;
        vec3 kD = vec3(1.0) - kS;
        kD *= 1.0 - metallic;

        vec3 numerator = NDF * G * F;
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
        vec3 specular = numerator / denominator;

        float NdotL = max(dot(N, L), 0.0);
        Lo += (kD * albedo / PI + specular) * radiance * NdotL;
    }

    vec3 ambient = vec3(0.03) * albedo * ao;
    vec3 color = ambient + Lo + emissive;

    FragColor = vec4(vec3(color), 1.0);
}
#version 450 core
out vec4 FragColor;

uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform sampler2D metallicMap;
uniform sampler2D roughnessMap;
uniform sampler2D aoMap;
uniform sampler2D emissiveMap;

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

in vec2 TexCoords;
in vec3 WorldPos;
in vec3 Normal;

in vec3 camPos;

const float PI = 3.14159265359;

/* vec3 albedo = vec3(0.5f, 0.0f, 0.0f);
float metallic = 0.2;
float roughness = 0.5;
float ao = 1.0f; */

//REMOVE this is for testing purposes;
vec3 getNormalFromMap()
{
    vec3 tangentNormal = texture(normalMap, TexCoords).xyz * 2.0 - 1.0;

    vec3 Q1  = dFdx(WorldPos);
    vec3 Q2  = dFdy(WorldPos);
    vec2 st1 = dFdx(TexCoords);
    vec2 st2 = dFdy(TexCoords);

    vec3 N   = normalize(Normal);
    vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
    vec3 B  = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}

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
    vec3 albedo = texture(albedoMap, TexCoords).rgb;
    float metallic = texture(metallicMap, TexCoords).b;
    float roughness = texture(roughnessMap, TexCoords).g;
    float ao = texture(aoMap, TexCoords).r;

    //vec3 N = normalize(Normal);
    vec3 N = getNormalFromMap();
    vec3 V = normalize(camPos - WorldPos);

    vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedo, metallic);

    vec3 Lo = vec3(0.0);
    for(int i = 0; i < lightCount; i++)
    {
        vec3 L = normalize(lights[i].position - WorldPos);
        vec3 H = normalize(V + L);
        float distance = length(lights[i].position - WorldPos);
        float attenuation = 1.0 / (distance * distance);
        vec3 radiance = lights[i].color * attenuation * lights[i].intensity;

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
    vec3 color = ambient + Lo;

    FragColor = vec4(vec3(color), 1.0);
}
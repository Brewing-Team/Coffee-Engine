#version 450 core
out vec4 FragColor;

struct VertexData
{
    vec2 TexCoords;
    vec3 Normal;
    vec3 WorldPos;
    vec3 camPos;
    mat3 TBN;
};

layout (location = 0) in VertexData VertexInput;

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

const float PI = 3.14159265359;

/* vec3 albedo = vec3(0.5f, 0.0f, 0.0f);
float metallic = 0.2;
float roughness = 0.5;
float ao = 1.0f; */

vec3 getNormalFromMap()
{
/*     vec3 tangentNormal = texture(normalMap, TexCoords).xyz * 2.0 - 1.0;

    vec3 Q1  = dFdx(WorldPos);
    vec3 Q2  = dFdy(WorldPos);
    vec2 st1 = dFdx(TexCoords);
    vec2 st2 = dFdy(TexCoords);

    vec3 N   = normalize(Normal);
    vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
    vec3 B  = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal); */

    vec3 normal = texture(normalMap, VertexInput.TexCoords).rgb * 2.0 - 1.0;
        return normalize(VertexInput.TBN * normal);
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
    vec3 albedo = texture(albedoMap, VertexInput.TexCoords).rgb;
    float metallic = texture(metallicMap, VertexInput.TexCoords).b;
    float roughness = texture(roughnessMap, VertexInput.TexCoords).g;
    float ao = texture(aoMap, VertexInput.TexCoords).r;

    //vec3 N = normalize(Normal);
    vec3 N = getNormalFromMap();
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
    vec3 color = ambient + Lo;

    FragColor = vec4(vec3(color), 1.0);
}
﻿// StandardShader.inl
#pragma once

const char* standardShaderSource = R""(
#[vertex]

#version 450 core
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormals;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;
layout (location = 5) in ivec4 aBoneIDs;
layout (location = 6) in vec4 aBoneWeights;

layout (std140, binding = 0) uniform camera
{
    mat4 projection;
    mat4 view;
    vec3 cameraPos;
};

struct VertexData
{
    vec2 TexCoords;
    vec3 Normal;
    vec3 WorldPos;
    vec3 camPos;
    mat3 TBN;
    vec4 FragPosLightSpace[4];
};

layout (location = 2) out VertexData Output;

uniform mat4 model;
uniform mat3 normalMatrix;

uniform mat4 lightSpaceMatrices[4];

uniform bool animated;
const int MAX_BONES = 100;
const int MAX_BONE_INFLUENCE = 4;
uniform mat4 finalBonesMatrices[MAX_BONES];

vec4 applyBoneTransform(vec4 pos)
{
    vec4 result = vec4(0.0f);
    for (int i = 0; i < MAX_BONE_INFLUENCE; ++i)
    {
        if (aBoneIDs[i] == -1)
            continue;
        if (aBoneIDs[i] >= MAX_BONES)
        {
            result = pos;
            break;
        }
        result += aBoneWeights[i] * (finalBonesMatrices[aBoneIDs[i]] * pos);
    }
    return result;
}

void main()
{
    vec4 totalPosition = vec4(0.0);
    vec3 totalNormal = vec3(0.0);

    if (animated)
    {
        totalPosition = applyBoneTransform(vec4(aPosition, 1.0f));
        totalNormal = normalize(applyBoneTransform(vec4(aNormals, 0.0))).xyz;
    }
    else
    {
        totalPosition = vec4(aPosition, 1.0f);
        totalNormal = aNormals;
    }

    Output.WorldPos = vec3(model * totalPosition);
    Output.Normal = normalMatrix * totalNormal;
    Output.camPos = cameraPos;
    Output.TexCoords = aTexCoord;

    for (int i = 0; i < 4; i++)
    {
        Output.FragPosLightSpace[i] = lightSpaceMatrices[i] * vec4(Output.WorldPos, 1.0);
    }

    gl_Position = projection * view * vec4(Output.WorldPos, 1.0);

    // Tangent space matrix

    //There is other way that is more efficient that is converting the lightPos and viewPos to tangent space in the vertex shader
    //and then pass them to the fragment shader. But this way is more simple and easy to understand + for PBR is better to transform
    //the normal map to view space + im lazy to move the lights to the vertex shader

    vec3 T = normalize(vec3(model * vec4(aTangent, 0.0)));
    vec3 B = normalize(vec3(model * vec4(aBitangent, 0.0)));
    vec3 N = normalize(vec3(model * vec4(totalNormal, 0.0)));

    Output.TBN = mat3(T, B, N);
}

#[fragment]

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
    vec4 FragPosLightSpace[4];
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

    int transparencyMode;
    // 0 = opaque
    // 1 = alpha
    // 2 = alpha cutoff
    float alphaCutoff;

    int hasAlbedo;
    int hasNormal;
    int hasMetallic;
    int hasRoughness;
    int hasAO;
    int hasEmissive;
};

uniform Material material;

uniform bool ditheringEnabled;
uniform float ditheringMinDistance;
uniform float ditheringMaxDistance;
uniform float ditheringCircleSize;
uniform vec3 camViewDir;

uniform samplerCube irradianceMap;
uniform samplerCube prefilterMap;
uniform sampler2D brdfLUT;

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
    float cone_attenuation;

    int type;

    // Shadows
    bool shadow;
    float shadowBias;
    float shadowMaxDistance;
};

layout (std140, binding = 1) uniform RenderData
{
    Light lights[MAX_LIGHTS];
    int lightCount;
};

uniform sampler2D shadowMaps[4];

uniform bool showNormals;

const float PI = 3.14159265359;

#define DITHER_PATTERN ditherGradientNoise

float ditherGradientNoise(vec2 uv) {
    const vec3 magic = vec3(0.06711056, 0.00583715, 52.9829189);
    return fract(magic.z * fract(dot(uv, magic.xy)));
}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
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

float ShadowCalculation(int lightIdx)
{
    // perform perspective divide
    vec4 fragPosLightSpace = VertexInput.FragPosLightSpace[lightIdx];
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;

    if (!lights[lightIdx].shadow)
        return 0.0;
    
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMaps[lightIdx], projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // calculate bias (based on depth map resolution and slope)
    vec3 normal = normalize(VertexInput.Normal);
    vec3 lightDir = normalize(lights[lightIdx].position - VertexInput.WorldPos);
    float bias = max(lights[lightIdx].shadowBias * (1.0 - dot(normal, lightDir)), 0.005);
    // check whether current frag pos is in shadow
    // float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
    // PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMaps[lightIdx], 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMaps[lightIdx], projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
    
    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if(projCoords.z > 1.0)
        shadow = 0.0;
        
    return shadow;
}

void main()
{
    vec3 albedo = material.hasAlbedo * (texture(material.albedoMap, VertexInput.TexCoords).rgb * material.color.rgb) + (1 - material.hasAlbedo) * material.color.rgb;

    float alpha = 1.0;
    if (material.transparencyMode == 1) {
        alpha = material.hasAlbedo * (texture(material.albedoMap, VertexInput.TexCoords).a) + (1 - material.hasAlbedo) * material.color.a;
    }
    else if (material.transparencyMode == 2) {
        alpha = material.hasAlbedo * (texture(material.albedoMap, VertexInput.TexCoords).a) + (1 - material.hasAlbedo) * material.color.a;
        if (alpha < material.alphaCutoff) {
            discard;
        }
    }

    if (ditheringEnabled) {
        float distance = length(VertexInput.camPos - VertexInput.WorldPos);

        vec3 camToFrag = normalize(VertexInput.WorldPos - VertexInput.camPos);
        float alignment = dot(camToFrag, camViewDir);

        float threshold = 1.0 - (ditheringCircleSize / 10);

        if (alignment >= threshold) {
            if (distance <= ditheringMaxDistance) {
                float fadeAlpha = 1.0;

                if (distance <= ditheringMinDistance) {
                    discard;
                } else {
                    fadeAlpha = (distance - ditheringMinDistance) / (ditheringMaxDistance - ditheringMinDistance);
                    fadeAlpha = smoothstep(0.0, 1.0, fadeAlpha);

                    float ditherThreshold = DITHER_PATTERN(gl_FragCoord.xy);

                    if (fadeAlpha < ditherThreshold) {
                        discard;
                    }
                }
            }
        }
    }

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
    vec3 R = reflect(-V, N);

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

            float shadow = ShadowCalculation(i);
            radiance *= (1.0 - shadow);
        }
        else if(lights[i].type == 1)
        {
            /*====Point Light====*/

            vec3 lightDir = normalize(lights[i].position - VertexInput.WorldPos);
            float distance = length(lights[i].position - VertexInput.WorldPos);

            if(distance > lights[i].range)
                continue;

            float attenuation = getOmniAttenuation(distance, 1.0 / lights[i].range, lights[i].attenuation);
            attenuation *= max(0.0, dot(N, lightDir));

            if(attenuation <= 0.0001)
                continue;

            L = lightDir;
            radiance = lights[i].color * attenuation * lights[i].intensity;
        }
        else if(lights[i].type == 2)
        {
            /*====Spot Light====*/
                        
/*             vec3 lightDir = normalize(lights[i].position - VertexInput.WorldPos);
                        
            // check if lighting is inside the spotlight cone
            float theta = dot(lightDir, normalize(-lights[i].direction));
                        
            if(theta > lights[i].angle) // Compare theta with lights[i].angle
            {
                float distance = length(lights[i].position - VertexInput.WorldPos);
                float attenuation = getOmniAttenuation(distance, 1.0 / lights[i].range, lights[i].attenuation);
                attenuation *= max(0.0, dot(N, lightDir));
                
                // Calculate spot rim and cone attenuation
                float spot_rim = max(0.0001, (1.0 - theta) / (1.0 - lights[i].angle));
                attenuation *= 1.0 - pow(spot_rim, lights[i].cone_attenuation);
            
                if(attenuation <= 0.0001)
                    continue;
            
                L = lightDir;
                radiance = lights[i].color * attenuation * lights[i].intensity;
            } */

            // Temporal implementation
            vec3 lightDir = normalize(lights[i].position - VertexInput.WorldPos);
            
            // check if lighting is inside the spotlight cone
            float theta = dot(lightDir, normalize(-lights[i].direction)); 
            float epsilon = (cos(radians(lights[i].angle)) - cos(radians(lights[i].cone_attenuation)));
            float intensity = clamp((theta - cos(radians(lights[i].cone_attenuation))) / epsilon, 0.0, 1.0);
            
            // attenuation
            float distance = length(lights[i].position - VertexInput.WorldPos);
            float attenuation = 1.0 / (1.0 + 0.09 * distance + 0.032 * (distance * distance));
            attenuation *= max(0.0, dot(N, lightDir)) * intensity;
            
            if(attenuation <= 0.0001)
                continue;
            
            L = lightDir;
            radiance = lights[i].color * attenuation * lights[i].intensity;
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

    //vec3 ambient = vec3(0.03) * albedo * ao;
    // ambient lighting (we now use IBL as the ambient term)
    vec3 F = fresnelSchlickRoughness(max(dot(N, V), 0.0), F0, roughness);
    
    vec3 kS = F;
    vec3 kD = 1.0 - kS;
    kD *= 1.0 - metallic;	  
    
    vec3 irradiance = texture(irradianceMap, N).rgb;
    vec3 diffuse = irradiance * albedo;

    const float MAX_REFLECTION_LOD = 4.0;
    vec3 prefilteredColor = textureLod(prefilterMap, R, roughness * MAX_REFLECTION_LOD).rgb;
    vec2 brdf = texture(brdfLUT, vec2(max(dot(N, V), 0.0), roughness)).rg;
    vec3 specular = prefilteredColor * (F * brdf.x + brdf.y);

    vec3 ambient = (kD * diffuse + specular) * ao;
    vec3 color = ambient + Lo + emissive;

    FragColor = vec4(color, alpha);
    EntityID = vec4(entityID, 1.0f); //set the alpha to 0

    //REMOVE: This is for the first release of the engine it should be handled differently
    if(showNormals)
    {
        FragColor = vec4((N * 0.5) + 0.5, 1.0);
        return;
    }
}
)"";
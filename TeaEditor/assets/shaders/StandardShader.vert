#version 450 core
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormals;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

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
};

layout (location = 0) out VertexData Output;

uniform mat4 model;
uniform mat3 normalMatrix;

void main()
{
    Output.WorldPos = vec3(model * vec4(aPosition, 1.0));
    Output.Normal = normalMatrix * aNormals;
    Output.camPos = cameraPos;
    Output.TexCoords = aTexCoord;

    gl_Position = projection * view * vec4(Output.WorldPos, 1.0);

    // Tangent space matrix

    //There is other way that is more efficient that is converting the lightPos and viewPos to tangent space in the vertex shader
    //and then pass them to the fragment shader. But this way is more simple and easy to understand + for PBR is better to transform
    //the normal map to view space + im lazy to move the lights to the vertex shader

    vec3 T = normalize(vec3(model * vec4(aTangent, 0.0)));
    vec3 B = normalize(vec3(model * vec4(aBitangent, 0.0)));
    vec3 N = normalize(vec3(model * vec4(aNormals, 0.0)));

    Output.TBN = mat3(T, B, N);
}

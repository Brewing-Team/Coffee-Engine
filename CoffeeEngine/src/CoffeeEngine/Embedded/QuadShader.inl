// QuadShader.inl
#pragma once

const char* quadShaderSource = R""(
#[vertex]

#version 450 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in float aTexIndex;
layout (location = 4) in float aTilingFactor;

layout (std140, binding = 0) uniform camera
{
    mat4 projection;
    mat4 view;
    vec3 cameraPos;
};

struct VertexData
{
    vec4 Color;
    vec2 TexCoord;
};

layout (location = 2) out VertexData Output;

void main()
{
    Output.Color = aColor;
    Output.TexCoord = aTexCoord;

    gl_Position = projection * view * vec4(aPosition, 1.0);
}

#[fragment]
#version 450 core

layout(location = 0) out vec4 FragColor;
layout(location = 1) out vec4 EntityID;

uniform vec3 entityID;

struct VertexData
{
    vec4 Color;
    vec2 TexCoord;
};

layout (location = 2) in VertexData Input;

void main()
{
    FragColor = Input.Color;
    EntityID = vec4(entityID, 1.0f); //set the alpha to 0
}
)"";
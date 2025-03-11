// LineShader.inl
#pragma once

const char* lineShaderSource = R"(
#[vertex]

#version 450 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec3 aEntityID;

layout (std140, binding = 0) uniform camera
{
    mat4 projection;
    mat4 view;
    vec3 cameraPos;
};

out vec4 Color;

layout (location = 0) out flat vec3 entityID;

void main()
{
    Color = aColor;
    gl_Position = projection * view * vec4(aPosition, 1.0);
    entityID = aEntityID;
}


#[fragment]

#version 450 core
layout(location = 0) out vec4 FragColor;
layout(location = 1) out vec4 EntityID;

in vec4 Color;

layout (location = 0) in flat vec3 entityID;

void main()
{
    FragColor = Color;
    EntityID = vec4(entityID, 1.0);
}
)";
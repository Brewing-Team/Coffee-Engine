// MissingShader.inl
#pragma once

const char* debugLineShaderSource = R"(
#[vertex]

#version 450 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec4 aColor;

layout (std140, binding = 0) uniform camera
{
    mat4 projection;
    mat4 view;
    vec3 cameraPos;
};

out vec4 Color;

void main()
{
    Color = aColor;
    gl_Position = projection * view * vec4(aPosition, 1.0);
}


#[fragment]

#version 450 core
out vec4 FragColor;

in vec4 Color;

void main()
{
    FragColor = Color;
}
)";
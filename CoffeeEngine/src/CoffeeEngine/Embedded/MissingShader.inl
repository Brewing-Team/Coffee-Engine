// MissingShader.inl
#pragma once

const char* missingShaderSource = R"(
#[vertex]

#version 450 core
layout (location = 0) in vec3 aPosition;

layout (std140, binding = 0) uniform camera
{
    mat4 projection;
    mat4 view;
};

uniform mat4 model;

void main()
{
    gl_Position = projection * view * model * vec4(aPosition, 1.0);
}

#[fragment]

#version 450 core
layout(location = 0) out vec4 FragColor;
layout(location = 1) out vec4 EntityID;

uniform vec3 entityID;

void main()
{
    FragColor = vec4(vec3(1.0, 0.0, 1.0), 1.0);
    EntityID = vec4(entityID, 1.0f); //set the alpha to 0
}
)";
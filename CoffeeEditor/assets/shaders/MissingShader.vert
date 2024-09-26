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

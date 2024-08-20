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

out vec2 TexCoords;
out vec3 Normal;
out vec3 WorldPos;
out vec3 camPos;

uniform mat4 model;
uniform mat3 normalMatrix;

void main()
{
    WorldPos = vec3(model * vec4(aPosition, 1.0));
    Normal = normalMatrix * aNormals;
    camPos = cameraPos;
    TexCoords = aTexCoord;

    gl_Position = projection * view * vec4(WorldPos, 1.0);
}

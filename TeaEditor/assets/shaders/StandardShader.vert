#version 450 core
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormals;
layout (location = 4) in vec3 aTangent;
layout (location = 5) in vec3 aBitangent;

layout (std140, binding = 0) uniform camera
{
    mat4 projection;
    mat4 view;
    vec3 cameraPos;
};

out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;
out vec3 viewPos;

uniform mat4 model;

void main()
{
    gl_Position = projection * view * model * vec4(aPosition, 1.0);
    FragPos = vec3(model * vec4(aPosition, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormals; //TODO: inversion is costly for shaders, move this to the CPU and send it like the uniform model
    viewPos = cameraPos;
    TexCoord = aTexCoord;
}

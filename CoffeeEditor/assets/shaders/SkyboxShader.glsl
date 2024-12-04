#[vertex]

#version 450 core

layout (location = 0) in vec3 aPosition;

layout (std140, binding = 0) uniform camera
{
    mat4 projection;
    mat4 view;
    vec3 cameraPos;
};

out vec3 TexCoord;

void main()
{
    mat4 view2 = mat4(mat3(view));

    TexCoord = aPosition;
    gl_Position = projection * view2 * vec4(aPosition, 1.0);
}

#[fragment]

#version 450 core
out vec4 FragColor;

in vec3 TexCoord;

uniform samplerCube skybox;

void main()
{
    FragColor = texture(skybox, TexCoord);
}
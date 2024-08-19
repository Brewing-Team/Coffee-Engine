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

out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;
out vec3 camPos;
out mat3 TBN;

uniform mat4 model;
uniform mat3 normalMatrix;

void main()
{
    FragPos = vec3(model * vec4(aPosition, 1.0));
    Normal = normalMatrix * aNormals;
    camPos = cameraPos;
    TexCoord = aTexCoord;

    gl_Position = projection * view * vec4(FragPos, 1.0);

    // Tangent space matrix
    vec3 T = normalize(vec3(model * vec4(aTangent, 0.0)));
    vec3 B = normalize(vec3(model * vec4(aBitangent, 0.0)));
    vec3 N = normalize(vec3(model * vec4(aNormals, 0.0)));

    TBN = mat3(T, B, N);
}

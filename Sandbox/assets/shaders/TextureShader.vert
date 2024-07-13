#version 450 core
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 ourColor;
out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec2 textureOffset;
uniform float textureScale;

void main()
{
    gl_Position = projection * view * model * vec4(aPosition, 1.0);
    ourColor = aColor;
    TexCoord = (aTexCoord + textureOffset) * textureScale;
}

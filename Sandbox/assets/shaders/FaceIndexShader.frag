#version 450 core

out vec4 FragColor;

void main()
{
    vec3 color = vec3(
        fract(sin(float(gl_PrimitiveID) * 12.9898) * 43758.5453),
        fract(sin(float(gl_PrimitiveID) * 4.1414) * 43758.5453),
        fract(sin(float(gl_PrimitiveID) * 7.7777) * 43758.5453)
    );
    FragColor = vec4(color, 1.0);
}

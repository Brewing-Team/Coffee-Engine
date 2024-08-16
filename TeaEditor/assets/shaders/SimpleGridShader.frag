#version 450 core
out vec4 FragColor;

in vec2 TexCoord;

void main()
{
    float lineWidth = 0.02;
    float gridSize = 1000;

    vec2 uv = TexCoord * gridSize;

    vec2 lineAA = fwidth(uv);
    vec2 gridUV = 1.0 - abs(fract(uv) * 2.0 - 1.0);
    vec2 grid2 = smoothstep(lineWidth + lineAA, lineWidth - lineAA, gridUV);
    float grid = grid2.x * (1.0 - grid2.y) + grid2.y;

    FragColor = vec4(grid * 0.75);
}
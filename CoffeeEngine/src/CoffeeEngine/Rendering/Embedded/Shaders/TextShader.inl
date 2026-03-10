// TextShader.inl
#pragma once

const char* textShaderSource = R""(
#[vertex]

#version 450 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexCoord;
layout (location = 5) in vec3 aEntityID;

layout (std140, binding = 0) uniform camera
{
    mat4 projection;
    mat4 view;
    vec3 cameraPos;
};

struct VertexData
{
    vec4 Color;
    vec2 TexCoord;
};

layout (location = 0) out VertexData Output;
layout (location = 4) out flat vec3 entityID;

void main()
{
    Output.Color = aColor;
    Output.TexCoord = aTexCoord;
    entityID = aEntityID;

    gl_Position = projection * view * vec4(aPosition, 1.0);
}

#[fragment]
#version 450 core

layout(location = 0) out vec4 FragColor;
layout(location = 1) out vec4 EntityID;

struct VertexData
{
    vec4 Color;
    vec2 TexCoord;
};

layout (location = 0) in VertexData Input;
layout (location = 4) in flat vec3 entityID;

layout (binding = 0) uniform sampler2D u_FontAtlas;

float screenPxRange() {
	const float pxRange = 2.0; // set to distance field's pixel range
    vec2 unitRange = vec2(pxRange)/vec2(textureSize(u_FontAtlas, 0));
    vec2 screenTexSize = vec2(1.0)/fwidth(Input.TexCoord);
    return max(0.5*dot(unitRange, screenTexSize), 1.0);
}

float median(float r, float g, float b) {
    return max(min(r, g), min(max(r, g), b));
}

void main()
{
    vec4 texColor = Input.Color * texture(u_FontAtlas, Input.TexCoord);

	vec3 msd = texture(u_FontAtlas, Input.TexCoord).rgb;
    float sd = median(msd.r, msd.g, msd.b);
    float screenPxDistance = screenPxRange()*(sd - 0.5);
    float opacity = clamp(screenPxDistance + 0.5, 0.0, 1.0);
	if (opacity == 0.0)
		discard;

	vec4 bgColor = vec4(0.0);
    FragColor = mix(bgColor, Input.Color, opacity);
	if (FragColor.a == 0.0)
		discard;

    EntityID = vec4(entityID, 1.0f);
}
)"";
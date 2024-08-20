#version 450 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D screenTexture;
uniform float exposure;

//Godot aces tonemap for testing
vec3 tonemapAces(vec3 color, float white) {
	const float exposure_bias = 1.8f;
	const float A = 0.0245786f;
	const float B = 0.000090537f;
	const float C = 0.983729f;
	const float D = 0.432951f;
	const float E = 0.238081f;

	// Exposure bias baked into transform to save shader instructions. Equivalent to `color *= exposure_bias`
	const mat3 rgb_to_rrt = mat3(
			vec3(0.59719f * exposure_bias, 0.35458f * exposure_bias, 0.04823f * exposure_bias),
			vec3(0.07600f * exposure_bias, 0.90834f * exposure_bias, 0.01566f * exposure_bias),
			vec3(0.02840f * exposure_bias, 0.13383f * exposure_bias, 0.83777f * exposure_bias));

	const mat3 odt_to_rgb = mat3(
			vec3(1.60475f, -0.53108f, -0.07367f),
			vec3(-0.10208f, 1.10813f, -0.00605f),
			vec3(-0.00327f, -0.07276f, 1.07602f));

	color *= rgb_to_rrt;
	vec3 color_tonemapped = (color * (color + A) - B) / (color * (C * color + D) + E);
	color_tonemapped *= odt_to_rgb;

	white *= exposure_bias;
	float white_tonemapped = (white * (white + A) - B) / (white * (C * white + D) + E);

	return color_tonemapped / white_tonemapped;
}

void main()
{
    float gamma = 2.2;

    vec3 hdrColor = texture(screenTexture, TexCoord).rgb;
    vec3 toneMappedColor;

    //Reinhard tone mapping
	//hdrColor *= exposure/(1. + hdrColor / exposure);
    toneMappedColor = vec3(1.0) - exp(-hdrColor * exposure);

    //Filmic tone mapping
    /* color = max(vec3(0.), color - vec3(0.004));
	color = (color * (6.2 * color + .5)) / (color * (6.2 * color + 1.7) + 0.06); */

    //ACES tone mapping
    //color = tonemap_aces(color, exposure);

    toneMappedColor = pow(toneMappedColor, vec3(1.0 / gamma));
    
    FragColor = vec4(toneMappedColor, 1.0);
}

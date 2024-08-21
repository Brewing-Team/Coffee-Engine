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

//-------------Agx Tone Mapping----------------//
//https://www.shadertoy.com/view/Dt3XDr

vec3 xyYToXYZ(vec3 xyY)
{
    float Y = xyY.z;
    float X = (xyY.x * Y) / xyY.y;
    float Z = ((1.0f - xyY.x - xyY.y) * Y) / xyY.y;

    return vec3(X, Y, Z);
}

vec3 Unproject(vec2 xy)
{
    return xyYToXYZ(vec3(xy.x, xy.y, 1));				
}

mat3 PrimariesToMatrix(vec2 xy_red, vec2 xy_green, vec2 xy_blue, vec2 xy_white)
{
    vec3 XYZ_red = Unproject(xy_red);
    vec3 XYZ_green = Unproject(xy_green);
    vec3 XYZ_blue = Unproject(xy_blue);
    vec3 XYZ_white = Unproject(xy_white);

    mat3 temp = mat3(XYZ_red.x,	  1.0f, XYZ_red.z,
                     XYZ_green.x, 1.0f, XYZ_green.z,
                     XYZ_blue.x,  1.0f, XYZ_blue.z);
    vec3 scale = inverse(temp) * XYZ_white;

    return mat3(XYZ_red * scale.x, XYZ_green * scale.y, XYZ_blue * scale.z);
}

mat3 ComputeCompressionMatrix(vec2 xyR, vec2 xyG, vec2 xyB, vec2 xyW, float compression)
{
    float scale_factor = 1.0f / (1.0f - compression);
    vec2 R = mix(xyW, xyR, scale_factor);
    vec2 G = mix(xyW, xyG, scale_factor);
    vec2 B = mix(xyW, xyB, scale_factor);
    vec2 W = xyW;

    return PrimariesToMatrix(R, G, B, W);
}

const float P = 1.0f;
const float l = 0.018f;
const float COMPRESSION = 0.15f;

const float SATURATION = 1.0f;

float DualSection(float x)
{
	// Length of linear section
	float S = (P * l);
	if (x < S) {
		return x;
	} else {
		float C = P / (P - S);
		return P - (P - S) * exp((-C * (x - S)) / P);
	}
}

vec3 DualSection(vec3 x)
{
	x.x = DualSection(x.x);
	x.y = DualSection(x.y);
	x.z = DualSection(x.z);
	return x;
}

vec3 toneMapAgx(vec3 color, float exposure) {

	vec3 resultColor = max(color, 0.0f) * pow(2.0f, exposure);

	mat3 sRGB_to_XYZ = PrimariesToMatrix(vec2(0.64,0.33),
                                         vec2(0.3,0.6), 
                                         vec2(0.15,0.06), 
                                         vec2(0.3127, 0.3290));
	
	mat3 adjusted_to_XYZ = ComputeCompressionMatrix(vec2(0.64,0.33),
												vec2(0.3,0.6), 
												vec2(0.15,0.06), 
												vec2(0.3127, 0.3290), COMPRESSION);
	
	mat3 XYZ_to_adjusted = inverse(adjusted_to_XYZ);
	mat3 sRGB_to_adjusted = sRGB_to_XYZ * XYZ_to_adjusted;

	resultColor = sRGB_to_adjusted * resultColor;
	resultColor = clamp(DualSection(resultColor), 0.0f, 1.0f);

	// Apply look in compressed gamut working space.
    vec3 luminanceWeight = vec3(0.2126729f,  0.7151522f,  0.0721750f);
    vec3 desaturation = vec3(dot(resultColor, luminanceWeight));
    resultColor = mix(desaturation, resultColor, SATURATION);
    resultColor = clamp(resultColor, 0.f, 1.f);

    resultColor = inverse(sRGB_to_adjusted) * resultColor;

	return resultColor;
}

//---------------------------------------------//

//https://iolite-engine.com/blog_posts/minimal_agx_implementation

#define AGX_LOOK 0

// Mean error^2: 3.6705141e-06
vec3 agxDefaultContrastApprox(vec3 x) {
  vec3 x2 = x * x;
  vec3 x4 = x2 * x2;
  
  return + 15.5     * x4 * x2
         - 40.14    * x4 * x
         + 31.96    * x4
         - 6.868    * x2 * x
         + 0.4298   * x2
         + 0.1191   * x
         - 0.00232;
}

vec3 agx(vec3 val) {
  const mat3 agx_mat = mat3(
    0.842479062253094, 0.0423282422610123, 0.0423756549057051,
    0.0784335999999992,  0.878468636469772,  0.0784336,
    0.0792237451477643, 0.0791661274605434, 0.879142973793104);
    
  const float min_ev = -12.47393f;
  const float max_ev = 4.026069f;

  // Input transform
  val = agx_mat * val;
  
  // Log2 space encoding
  val = clamp(log2(val), min_ev, max_ev);
  val = (val - min_ev) / (max_ev - min_ev);
  
  // Apply sigmoid function approximation
  val = agxDefaultContrastApprox(val);

  return val;
}

vec3 agxEotf(vec3 val) {
  const mat3 agx_mat_inv = mat3(
    1.19687900512017, -0.0528968517574562, -0.0529716355144438,
    -0.0980208811401368, 1.15190312990417, -0.0980434501171241,
    -0.0990297440797205, -0.0989611768448433, 1.15107367264116);
    
  // Undo input transform
  val = agx_mat_inv * val;
  
  // sRGB IEC 61966-2-1 2.2 Exponent Reference EOTF Display
  //val = pow(val, vec3(2.2));

  return val;
}

vec3 agxLook(vec3 val) {
  const vec3 lw = vec3(0.2126, 0.7152, 0.0722);
  float luma = dot(val, lw);
  
  // Default
  vec3 offset = vec3(0.0);
  vec3 slope = vec3(1.0);
  vec3 power = vec3(1.0);
  float sat = 1.0;
 
#if AGX_LOOK == 1
  // Golden
  slope = vec3(1.0, 0.9, 0.5);
  power = vec3(0.8);
  sat = 0.8;
#elif AGX_LOOK == 2
  // Punchy
  slope = vec3(1.0);
  power = vec3(1.35, 1.35, 1.35);
  sat = 1.4;
#endif
  
  // ASC CDL
  val = pow(val * slope + offset, power);
  return luma + sat * (val - luma);
}

//---------------------------------------------//

void main()
{
    float gamma = 2.2;

    vec3 hdrColor = texture(screenTexture, TexCoord).rgb;
    vec3 toneMappedColor;

/*     if(gl_FragCoord.x < 559 && gl_FragCoord.y < 300) // Bottom left
	{
		toneMappedColor = toneMapAgx(hdrColor, 1.0);

	}
	else if(gl_FragCoord.x < 559 && gl_FragCoord.y > 300) // Top left
	{
		toneMappedColor = agxLook(hdrColor);
	}
	else if(gl_FragCoord.x > 559 && gl_FragCoord.y < 300) // Bottom right
	{
		toneMappedColor = tonemapAces(hdrColor, 1.0);
	}
	else if(gl_FragCoord.x > 559 && gl_FragCoord.y > 300) // Top right
	{
		toneMappedColor = (hdrColor * (6.2 * hdrColor + .5)) / (hdrColor * (6.2 * hdrColor + 1.7) + 0.06);
	} */

/* 	if(gl_FragCoord.x < 559)
	{
		toneMappedColor = toneMapAgx(hdrColor, 1.0);
	}
	else
	{
		toneMappedColor = tonemapAces(hdrColor, 1.0);
	} */

	toneMappedColor = toneMapAgx(hdrColor, exposure);

    toneMappedColor = pow(toneMappedColor, vec3(1.0 / gamma));
    
    FragColor = vec4(toneMappedColor, 1.0);
}

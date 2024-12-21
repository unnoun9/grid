uniform sampler2D texture1; // underlying layer
uniform sampler2D texture2; // upper layer
uniform float alpha;        // alpha value for blending

vec3 vivid_light(vec3 base, vec3 blend)
{
    vec3 result;
    result.r = (blend.r < 0.5) ? (1.0 - (1.0 - base.r) / (2.0 * blend.r + 0.0001)) : (base.r / (2.0 * (1.0 - blend.r) + 0.0001));
    result.g = (blend.g < 0.5) ? (1.0 - (1.0 - base.g) / (2.0 * blend.g + 0.0001)) : (base.g / (2.0 * (1.0 - blend.g) + 0.0001));
    result.b = (blend.b < 0.5) ? (1.0 - (1.0 - base.b) / (2.0 * blend.b + 0.0001)) : (base.b / (2.0 * (1.0 - blend.b) + 0.0001));
    return clamp(result, 0.0, 1.0);
}

void main()
{
    vec4 color1 = texture2D(texture1, gl_TexCoord[0].xy);
    vec4 color2 = texture2D(texture2, gl_TexCoord[0].xy);

    vec3 blended_color = vivid_light(color1.rgb, color2.rgb);

    gl_FragColor = vec4(mix(color1.rgb, blended_color, alpha), color1.a);
}
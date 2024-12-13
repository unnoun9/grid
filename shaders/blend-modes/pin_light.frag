uniform sampler2D texture1; // underlying layer
uniform sampler2D texture2; // upper layer
uniform float alpha; // alpha value for blending

void main()
{
    vec4 color1 = texture2D(texture1, gl_TexCoord[0].xy);
    vec4 color2 = texture2D(texture2, gl_TexCoord[0].xy);
    vec4 blendedColor = vec4(
        (color2.r > 0.5) ? max(color1.r, 2.0 * (color2.r - 0.5)) : min(color1.r, 2.0 * color2.r),
        (color2.g > 0.5) ? max(color1.g, 2.0 * (color2.g - 0.5)) : min(color1.g, 2.0 * color2.g),
        (color2.b > 0.5) ? max(color1.b, 2.0 * (color2.b - 0.5)) : min(color1.b, 2.0 * color2.b),
        color1.a
    );
    gl_FragColor = mix(color1, blendedColor, alpha);
}

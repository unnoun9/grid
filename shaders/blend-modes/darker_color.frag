uniform sampler2D texture1; // underlying layer
uniform sampler2D texture2; // upper layer
uniform float alpha; // alpha value for blending

float luminance(vec4 color) {
    return 0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b;
}

void main()
{
    vec4 color1 = texture2D(texture1, gl_TexCoord[0].xy);
    vec4 color2 = texture2D(texture2, gl_TexCoord[0].xy);
    vec4 blendedColor = luminance(color1) < luminance(color2) ? color1 : color2;
    gl_FragColor = mix(color1, blendedColor, alpha);
}

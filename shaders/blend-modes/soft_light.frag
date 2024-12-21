uniform sampler2D texture1; // underlying layer
uniform sampler2D texture2; // upper layer
uniform float alpha;        // alpha value for blending

float gw3c(float a)
{
    return (a <= 0.25) ? (((16.0 * a - 12.0) * a + 4.0) * a) : sqrt(a);
}

void main()
{
    vec4 color1 = texture2D(texture1, gl_TexCoord[0].xy);
    vec4 color2 = texture2D(texture2, gl_TexCoord[0].xy);

    vec4 blended_color = vec4(
        (color2.r <= 0.5) ? (color1.r - (1.0 - 2.0 * color2.r) * color1.r * (1.0 - color1.r)) : (color1.r + (2.0 * color2.r - 1.0) * (gw3c(color1.r) - color1.r)),
        (color2.g <= 0.5) ? (color1.g - (1.0 - 2.0 * color2.g) * color1.g * (1.0 - color1.g)) : (color1.g + (2.0 * color2.g - 1.0) * (gw3c(color1.g) - color1.g)),
        (color2.b <= 0.5) ? (color1.b - (1.0 - 2.0 * color2.b) * color1.b * (1.0 - color1.b)) : (color1.b + (2.0 * color2.b - 1.0) * (gw3c(color1.b) - color1.b)),
        color1.a
    );
    
    gl_FragColor = mix(color1, blended_color, alpha);
}

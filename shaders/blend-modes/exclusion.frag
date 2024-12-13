uniform sampler2D texture1; // underlying layer
uniform sampler2D texture2; // upper layer
uniform float alpha; // alpha value for blending

void main()
{
    vec4 color1 = texture2D(texture1, gl_TexCoord[0].xy);
    vec4 color2 = texture2D(texture2, gl_TexCoord[0].xy);
    vec4 blendedColor = 0.5 - 2.0 * (color1 - 0.5) * (color2 - 0.5);
    gl_FragColor = mix(color1, blendedColor, alpha);
}

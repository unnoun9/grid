uniform sampler2D texture1; // underlying layer
uniform sampler2D texture2; // upper layer
uniform float alpha; // alpha value for blending

vec3 set_hue(vec3 color, float hue)
{
    float U = cos(hue * 3.14159265 / 180.0);
    float W = sin(hue * 3.14159265 / 180.0);
    mat3 hueRotation = mat3(
        0.299 + 0.701 * U + 0.168 * W, 0.587 - 0.587 * U + 0.330 * W, 0.114 - 0.114 * U - 0.497 * W,
        0.299 - 0.299 * U - 0.328 * W, 0.587 + 0.413 * U + 0.035 * W, 0.114 - 0.114 * U + 0.292 * W,
        0.299 - 0.300 * U + 1.250 * W, 0.587 - 0.588 * U - 1.050 * W, 0.114 + 0.886 * U - 0.203 * W
    );
    return color * hueRotation;
}

void main()
{
    vec4 color1 = texture2D(texture1, gl_TexCoord[0].xy);
    vec4 color2 = texture2D(texture2, gl_TexCoord[0].xy);
    vec3 hueColor = set_hue(color1.rgb, atan(color2.g, color2.r) * 180.0 / 3.14159265);
    vec4 blendedColor = vec4(hueColor, color1.a);
    gl_FragColor = mix(color1, blendedColor, alpha);
}

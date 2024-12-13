uniform sampler2D texture1; // underlying layer
uniform sampler2D texture2; // upper layer
uniform float alpha; // alpha value for blending

float get_luminance(vec3 color)
{
    return dot(color, vec3(0.299, 0.587, 0.114));
}

vec3 set_lumninance(vec3 color, float luminance)
{
    float currentLuminance = get_luminance(color);
    return color * (luminance / currentLuminance);
}

void main()
{
    vec4 color1 = texture2D(texture1, gl_TexCoord[0].xy);
    vec4 color2 = texture2D(texture2, gl_TexCoord[0].xy);
    float luminance = get_luminance(color2.rgb);
    vec3 lumColor = set_lumninance(color1.rgb, luminance);
    vec4 blendedColor = vec4(lumColor, color1.a);
    gl_FragColor = mix(color1, blendedColor, alpha);
}

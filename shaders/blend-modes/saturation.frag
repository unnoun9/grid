uniform sampler2D texture1; // underlying layer
uniform sampler2D texture2; // upper layer
uniform float alpha; // alpha value for blending

float get_saturation(vec3 color)
{
    float maxVal = max(max(color.r, color.g), color.b);
    float minVal = min(min(color.r, color.g), color.b);
    return maxVal - minVal;
}

vec3 set_saturation(vec3 color, float saturation)
{
    float gray = dot(color, vec3(0.299, 0.587, 0.114));
    return mix(vec3(gray), color, saturation);
}

void main()
{
    vec4 color1 = texture2D(texture1, gl_TexCoord[0].xy);
    vec4 color2 = texture2D(texture2, gl_TexCoord[0].xy);
    float saturation = get_saturation(color2.rgb);
    vec3 satColor = set_saturation(color1.rgb, saturation);
    vec4 blendedColor = vec4(satColor, color1.a);
    gl_FragColor = mix(color1, blendedColor, alpha);
}

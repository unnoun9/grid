uniform sampler2D texture1; // underlying layer
uniform sampler2D texture2; // upper layer
uniform float alpha;        // alpha value for blending

float get_luminance(vec3 color)
{
    return dot(color, vec3(0.299, 0.587, 0.114));
}

vec3 set_luminance(vec3 color, float luminance)
{
    float current_luminance = get_luminance(color);
    return color * (luminance / current_luminance);
}

void main()
{
    vec4 color1 = texture2D(texture1, gl_TexCoord[0].xy);
    vec4 color2 = texture2D(texture2, gl_TexCoord[0].xy);

    float luminance = get_luminance(color2.rgb);
    vec3 lum_color = set_luminance(color1.rgb, luminance);
    vec4 blended_color = vec4(lum_color, color1.a);
    
    gl_FragColor = mix(color1, blended_color, alpha);
}

uniform sampler2D texture1; // underlying layer
uniform sampler2D texture2; // upper layer
uniform float alpha;        // alpha value for blending

vec3 subtract(vec3 base, vec3 blend)
{
    return max(base - blend, 0.0);
}

void main()
{
    vec4 color1 = texture2D(texture1, gl_TexCoord[0].xy);
    vec4 color2 = texture2D(texture2, gl_TexCoord[0].xy);

    vec3 blended_color = subtract(color1.rgb, color2.rgb);

    gl_FragColor = vec4(mix(color1.rgb, blended_color, alpha), color1.a);
}
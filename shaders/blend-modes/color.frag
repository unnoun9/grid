uniform sampler2D texture1; // underlying layer
uniform sampler2D texture2; // upper layer
uniform float alpha;        // alpha value for blending

vec3 set_color(vec3 color, vec3 hue_color)
{
    float gray = dot(color, vec3(0.299, 0.587, 0.114));
    return vec3(gray) + hue_color - vec3(dot(hue_color, vec3(0.299, 0.587, 0.114)));
}

void main()
{
    vec4 color1 = texture2D(texture1, gl_TexCoord[0].xy);
    vec4 color2 = texture2D(texture2, gl_TexCoord[0].xy);

    vec3 color = set_color(color1.rgb, color2.rgb);
    vec4 blended_color = vec4(color, color1.a);
    
    gl_FragColor = mix(color1, blended_color, alpha);
}

uniform sampler2D texture;

void main()
{
    vec4 color = texture2D(texture, gl_TexCoord[0].xy);

    if (color.a == 0.0)
    {
        gl_FragColor = color;
        return;
    }

    float r = dot(color.rgb, vec3(0.393, 0.769, 0.189));
    float g = dot(color.rgb, vec3(0.349, 0.686, 0.168));
    float b = dot(color.rgb, vec3(0.272, 0.534, 0.131));
    gl_FragColor = vec4(clamp(vec3(r, g, b), 0.0, 1.0), color.a);
}
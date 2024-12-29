uniform sampler2D texture;

void main()
{
    vec4 color = texture2D(texture, gl_TexCoord[0].xy);

    if (color.a == 0.0)
    {
        gl_FragColor = color;
        return;
    }

    color.rgb = vec3(1.0) - color.rgb;
    gl_FragColor = color;
}
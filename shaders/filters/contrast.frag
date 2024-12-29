uniform sampler2D texture;
uniform float contrast;

void main()
{
    vec4 color = texture2D(texture, gl_TexCoord[0].st);

    if (color.a == 0.0)
    {
        gl_FragColor = color;
        return;
    }

    float contrast_factor = (259 * (contrast + 255)) / (255 * (259 - contrast));
    color.rgb = clamp(contrast_factor * (color.rgb - 0.5) + 0.5, 0.0, 1.0);
    gl_FragColor = color;
}

